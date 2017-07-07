# Goddess Sound Visualizer

### Min unreal version
UE 4.15
Check **compile** section for more info.

### At glance
Video: https://www.youtube.com/watch?v=Zkd1riFMd-8
How to import and visualize: https://youtu.be/JOrl5a6-EhU

Blueprint example project UE4.16: [Here](https://github.com/AndreaCatania/GoddessSoundVisualizer/blob/master/GoddessSoundVis_4.16.rar?raw=true)

Allow you to create a visualizer of sound with blueprint. While other visualizer are not working out of editor this plugin works well.

This plugin uses low resources, because all data needed are precalculated during the importing of the wav file.

Controll your sound overtime using Play, Stop, Pause, Resume functions directly on Blueprint. Visualize the sound by catch **OnUpdateFrequency** event and then create the effect you want.

---

## How to install
// TODO

---

## How Works
After the installation of the plugin, you are able to import and visualize audio files.

I split this guide in two parts, the first part of guide talks about the visualizer class, while the second part talks about how to import frequency spectrum that is required by the visualizer class.

The class GoddessSoundVisualizer is the main class that allow you to visualize your sound. This class provides five functions that are used to manage the audio reproduction, And two events to update the scene actors each frame.

| Function | Description |
|---|---|
| **Play** | This function is used to reproduce and visualize the sound file, it accepts the sound and the relative frequency spectrum (I show you later how to get this) |
| **Stop** | Stop the sound and cannot be resumed, is necessary that you call again the *Play* function to set another sound. |
| **Pause** | Pause the current sound and can be resumed. |
| **Resume** | Resume a paused sound. |
| **Normalize** | This is a help function that allow you to normalize the frequency (look the example project) |


| Event | Description |
|---|---|
| **OnUpdateFrequency** | The core of this class is the event OnUpdateFrequency that is called for each frames and returns the frequency for the main channels (SubBass, Bass, LowMidrange, Midrange, UpperMidrange, Presence, Brilliance). ***(Note1)*** |
| **OnAudioFinished** | This event is fired when an audio ends. |

***(Note1)*** This event can be used to update the size of cylinder (frame by frame) as I show in the presentation video, of this library, or something different according to your needs.
(downloads the example project to see how I implemented the updating of the cylinders size).

### The trick of library
The reason for which I say that library is light is because the frequency spectrum is extrapolated from wav files during the importing in the editor. In fact during the importing is created a new file extension that contains the audio frequency value ready to be displayed.

To allow the editor to extrapolate the frequency spectrum of wav file it's necessary that you make a copy, of your "**YourAudio.wav**" file that you want import, in your directory and rename it with this extension "YourAudio.**wavfs**" now you can drag and drop this file into the editor.

That's it, now you can set this frequency spectrum in **Play** function and visualize the audio frequency spectrum as show in the video.

Check the example project.


---

## Compile
If you are using Windows 64bit with UE4.16 you can use directly the binaries that are in /Binaries folder.
If you are on other type of machine Win32 / Linux / etc... or other engine version you must compile the plugin.

---
## License
AGPL-3.0

---
# Donation
I've shared this library for free, however if it was useful for your project, or you like my work I'm glad to you if you support me with a donation [here My PayPall link](https://www.paypal.me/AndreaCatania/24.99eur)

---
## Credits
Thanks to eXifreXi that has explained me why others plugin are not working without editor, Mattiwatti helped me to get right information on how implement the importer, BlackFox for understanding wav file.
