/*==============================================================================

    Manage Audio [Audio.cpp]

    Author : Choi HyungJoon

==============================================================================*/
#include "Audio.h"
#include "Setting.h"
#include <assert.h>
#include <vector>
#include <mutex>
#include <algorithm>
#include <mmsystem.h>

using namespace std;

struct AUDIO
{
    IXAudio2SourceVoice* Source_Voice{};

    WAVEFORMATEX WaveFormat{};
    BYTE* SoundData{};
    int Length{};
    int PlayLength{};
};

static IXAudio2* X_audio{};
static IXAudio2MasteringVoice* Mastering_Voice{};

static AUDIO BGM_Arr[BGM_MAX]{};
static AUDIO SFX_Arr[SFX_MAX]{};

static vector<IXAudio2SourceVoice*> Active_SFX;
static mutex Voice_Mutex;

static int BGM_Volume_Level = SOUND_MAX;
static int SFX_Volume_Level = SOUND_MAX;

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
    VoiceCallback() {}
    ~VoiceCallback() {}

    void OnBufferEnd(void* pBufferContext)
    {
        IXAudio2SourceVoice* pSourceVoice = (IXAudio2SourceVoice*)pBufferContext;

        std::lock_guard<std::mutex> lock(Voice_Mutex);

        auto it = std::find(Active_SFX.begin(), Active_SFX.end(), pSourceVoice);

        if (it != Active_SFX.end())
            Active_SFX.erase(it);

        pSourceVoice->DestroyVoice();
    }

    void OnStreamEnd() {}
    void OnVoiceProcessingPassEnd() {}
    void OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
    void OnBufferStart(void* pBufferContext) {}
    void OnLoopEnd(void* pBufferContext) {}
    void OnVoiceError(void* pBufferContext, HRESULT Error) {}
};

static VoiceCallback SFX_Call_back;

void InitAudio()
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    XAudio2Create(&X_audio, 0);
    X_audio->CreateMasteringVoice(&Mastering_Voice);
}

void Un_InitAudio()
{
    StopAllSFX();

    for (int i = 0; i < BGM_MAX; i++)
        UnloadBGM(i);

    for (int i = 0; i < SFX_MAX; i++)
        UnloadSFX(i);

    Mastering_Voice->DestroyVoice();
    X_audio->Release();
    CoUninitialize();
}

static int LoadWaveFile(const char* FileName, AUDIO* audio, bool isBGM)
{
    int index = -1;
    int max_count = isBGM ? BGM_MAX : SFX_MAX;

    for (int i = 0; i < max_count; i++)
    {
        if (audio[i].SoundData == nullptr)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return -1;

    // Open 'wav' file
    HMMIO HMMIO_Open = mmioOpenA((LPSTR)FileName, NULL, MMIO_READ);
    if (!HMMIO_Open)
        return -1;

    // 'WAVE'チャンクに移動
    MMCKINFO Chunk_info{};
    Chunk_info.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    mmioDescend(HMMIO_Open, &Chunk_info, NULL, MMIO_FINDRIFF);

    // 'fmt 'チャンクに移動
    MMCKINFO MMCK_info{};
    MMCK_info.ckid = mmioFOURCC('f', 'm', 't', ' ');
    mmioDescend(HMMIO_Open, &MMCK_info, &Chunk_info, MMIO_FINDCHUNK);

    // Read Format Info
    mmioRead(HMMIO_Open, (HPSTR)&audio[index].WaveFormat, sizeof(audio[index].WaveFormat));
    mmioAscend(HMMIO_Open, &MMCK_info, 0);

    // 'data'チャンクに移動
    MMCKINFO Data_Chunk_info{};
    Data_Chunk_info.ckid = mmioFOURCC('d', 'a', 't', 'a');
    mmioDescend(HMMIO_Open, &Data_Chunk_info, &Chunk_info, MMIO_FINDCHUNK);

    // Read Sound data
    UINT32 Buffer_Len = Data_Chunk_info.cksize;
    audio[index].SoundData = new BYTE[Buffer_Len];
    mmioRead(HMMIO_Open, (HPSTR)audio[index].SoundData, Buffer_Len);

    audio[index].Length = Buffer_Len;
    audio[index].PlayLength = Buffer_Len / audio[index].WaveFormat.nBlockAlign;

    mmioClose(HMMIO_Open, 0);

    // If BGM, make Source Voice
    if (isBGM)
    {
        X_audio->CreateSourceVoice(&audio[index].Source_Voice, &audio[index].WaveFormat);
        assert(audio[index].Source_Voice);
    }

    return index;
}


int LoadBGM(const char* FileName)
{
    return LoadWaveFile(FileName, BGM_Arr, true);
}

int LoadSFX(const char* FileName)
{
    return LoadWaveFile(FileName, SFX_Arr, false);
}

void UnloadBGM(int Index)
{
    if (Index < 0 || Index >= BGM_MAX)
        return;

    if (!BGM_Arr[Index].SoundData)
        return;

    if (BGM_Arr[Index].Source_Voice)
    {
        BGM_Arr[Index].Source_Voice->Stop();
        BGM_Arr[Index].Source_Voice->DestroyVoice();
    }
    delete[] BGM_Arr[Index].SoundData;
    BGM_Arr[Index] = {};
}

void UnloadSFX(int Index)
{
    if (Index < 0 || Index >= SFX_MAX)
        return;

    if (!SFX_Arr[Index].SoundData)
        return;

    delete[] SFX_Arr[Index].SoundData;
    SFX_Arr[Index] = {};
}

void PlayBGM(int Index, bool Loop)
{
    if (Index < 0 || Index >= BGM_MAX || !BGM_Arr[Index].Source_Voice)
        return;

    BGM_Arr[Index].Source_Voice->Stop();
    BGM_Arr[Index].Source_Voice->FlushSourceBuffers();

    XAUDIO2_BUFFER buf{};
    buf.AudioBytes = BGM_Arr[Index].Length;
    buf.pAudioData = BGM_Arr[Index].SoundData;
    buf.PlayBegin = 0;
    buf.PlayLength = BGM_Arr[Index].PlayLength;

    if (Loop)
    {
        buf.LoopBegin = 0;
        buf.LoopLength = BGM_Arr[Index].PlayLength;
        buf.LoopCount = XAUDIO2_LOOP_INFINITE;
    }

    BGM_Arr[Index].Source_Voice->SubmitSourceBuffer(&buf, NULL);
    BGM_Arr[Index].Source_Voice->SetVolume(BGM_Volume_Level / 10.0f);
    BGM_Arr[Index].Source_Voice->Start();
}

void StopBGM(int Index)
{
    if (Index < 0 || Index >= BGM_MAX)
        return;

    if (BGM_Arr[Index].Source_Voice)
        BGM_Arr[Index].Source_Voice->Stop();
}

void PlaySFX(int Index)
{
    if (Index < 0 || Index >= SFX_MAX || !SFX_Arr[Index].SoundData)
        return;

    // If play SFX_Arr, make temp Source Voice
    IXAudio2SourceVoice* pSourceVoice{};
    X_audio->CreateSourceVoice(&pSourceVoice, &(SFX_Arr[Index].WaveFormat), 0, XAUDIO2_DEFAULT_FREQ_RATIO, &SFX_Call_back);

    XAUDIO2_BUFFER buf{};
    buf.AudioBytes = SFX_Arr[Index].Length;
    buf.pAudioData = SFX_Arr[Index].SoundData;
    buf.PlayBegin = 0;
    buf.PlayLength = SFX_Arr[Index].PlayLength;
    // Send callback pointer to context
    buf.pContext = pSourceVoice;

    pSourceVoice->SubmitSourceBuffer(&buf, NULL);
    pSourceVoice->SetVolume(SFX_Volume_Level / 10.0f);

    // Lock with mutex, add voice list
    {
        std::lock_guard<std::mutex> lock(Voice_Mutex);
        Active_SFX.push_back(pSourceVoice);
    }

    pSourceVoice->Start();
}

void StopAllSFX()
{
    // Lock with mutex, Stop all SFX_Arr and Destroy
    std::lock_guard<std::mutex> lock(Voice_Mutex);

    for (auto pVoice : Active_SFX)
    {
        pVoice->Stop();
        pVoice->DestroyVoice();
    }
    Active_SFX.clear();
}

void SetBGMVolume(int volume)
{
    BGM_Volume_Level = max(0, min(10, volume));
    UpdateBGMVolume();
}

void SetSFXVolume(int volume)
{
    SFX_Volume_Level = max(0, min(10, volume));
}

void UpdateBGMVolume()
{
    for (int i = 0; i < BGM_MAX; i++)
    {
        if (BGM_Arr[i].Source_Voice)
            BGM_Arr[i].Source_Voice->SetVolume(BGM_Volume_Level / static_cast<float>(SOUND_MAX));
    }
}