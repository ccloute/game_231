#include "audio.h"

#include <SDL3_mixer/SDL_mixer.h>

#include <fstream>
#include <stdexcept>
#include <algorithm>

Audio::Audio() {
    // Open default audio device (Mixer no longer works directly with
    // audio devices, SDL3 manages it now)
    SDL_AudioSpec spec{.format = SDL_AUDIO_F32, .channels = 2, .freq = 48000};
    device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (device == 0) {
        std::string msg{"Audio::Audio(): "};
        throw std::runtime_error(msg + SDL_GetError());
    }
    if (!MIX_Init()) {
        SDL_CloseAudioDevice(device);
        std::string msg{"Audio::Audio(): "};
        throw std::runtime_error(msg + SDL_GetError());
    }
    mixer = MIX_CreateMixerDevice(device, &spec);
    background_music = MIX_CreateTrack(mixer); 
}

Audio::~Audio() {
    // remove existing sounds
    MIX_DestroyMixer(mixer);
    SDL_CloseAudioDevice(device);
    MIX_Quit();
}

void Audio::load_sounds(const std::string& filename) {
    // remove existing sounds
    for (auto [name, sound] : sounds) {
        MIX_DestroyAudio(sound);
    }

    std::ifstream input{filename};
    if (!input) {
        throw std::runtime_error("Could not open filename: " + filename);
    }
    std::size_t i = filename.find('/');
    std::string parent_path{filename.substr(0, i + 1)};

    std::string name, file;
    while (input >> name >> file) {
        std::string full_path = parent_path + file;
        MIX_Audio* effect = MIX_LoadAudio(mixer, full_path.c_str(), true);
        if (!effect) {
            std::string msg{SDL_GetError()};
            throw std::runtime_error(msg + "\nUnable to load sound from " + full_path);
        }
        sounds[name] = effect;
    }
}

void Audio::play_sound(const std::string& sound_name, bool loop_forever_in_background) {
    auto sound = sounds.find(sound_name);
    if (sound == sounds.end()) {
        throw std::runtime_error("Cannot find sound " + sound_name);
    }

    if (loop_forever_in_background) {
        // SDL properties now control infinite looping
        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
        MIX_SetTrackAudio(background_music, sound->second);
        if(!MIX_PlayTrack(background_music, props)) {
            std::string msg{SDL_GetError()};
            throw std::runtime_error(msg + "\nBackground sound " + sound_name + " cannot be played.");
        }
    } else {
        if(!MIX_PlayAudio(mixer, sound->second)) {
            std::string msg{SDL_GetError()};
            throw std::runtime_error(msg + "\n" + sound_name + " cannot be played.");
        }
    }
}
