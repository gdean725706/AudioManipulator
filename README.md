# Audio Manipulator
! [am1_img](https://i.imgur.com/7WMaRet.png)
Audio Manipulator is a real-time audio processing application for standalone and mobile devices aimed at musicians, performers, and individuals alike, who wish to creatively process audio for recording and live performance. 

An X-Y based control system with user motion and modulation functionality provides dynamic control over a selection of in-built effects which utilise digital signal processing techniques to drastically alter incoming audio. 

## Installation
Audio Manipulator is still in development and not every Platform / DAW combination has been tested, so use at your own risk!

Builds for Windows, Mac & Android are published in the Releases section.

### Windows Standalone
Run **AudioManipulator.exe** file from anywhere on your computer. 
A suitable place would be `C:/Users/Documents/AudioManipulator`

### Windows VST Plugin (64 bit)
Copy the **AudioManipulator.dll** file into your VST Plugins directory.
Typically `C:/Program Files/VstPlugins` 

### Mac VST (64 bit)
Copy **“AudioManipulator.vst”** to your Mac VST Plugins folder. 
Typically `Library/Audio/Plugins/VST`

### Mac AU
Copy **AudioManipulator.component** to your Mac Components folder.
Typically `Library/Audio/Plugins/Components/`

### Android
Install the **AudioManipulator.apk** file on your Android Device.
You may have to enable *“Allow installers from unverified sources”* in your device security settings in order to do this. 

## Building
Audio Manipulator uses [JUCE](https://juce.com/) v5.1.2 and has build configurations for Windows, Mac, Android & iOS already set up.
Open the .jucer file and export the project to your selected IDE to build for that platform.

## Usage

![UI_IMG](https://i.imgur.com/Gtt7DKS.png)

### Effect Selection
The buttons along the top allow you to choose which effect is active on audio input. Dark grey means an effect is enabled and linked to the X-Y controller. 

### LFO Modulation 
This section contains 3 low frequency oscillators (LFOs) to modulate the X-Y position. The larger “rate” knob controls the speed of oscillation, while the smaller X and Y knobs control the amount of modulation applied. 

### XY Controller 
This is the main input controller for the effects processes. It is divided into two axes, vertical and horizontally, with each axis linked to a parameter for each effect. The names of the effects it is controlling are displayed along each axis line. 

### User Motion Controls
Here, 3 slots are available to record input motions made to the X-Y controller. Click on an empty slot to start recording. The block will turn red to indicate it is recording. Create a motion on the X-Y controller, and when you are done, click the block again. It will turn green to indicate that there is a motion recorded to the slot. Clicking a green slot will begin playback of the recorded motion in that slot. You can change the speed of the playback using the slider at the bottom of the user motion control section. Smoothing mode allows you to choose between linear and cubic interpolation methods for smoothing of user motion playback. 
### Sampler
The sampler section enables you to record up to 10 seconds of audio per slot for playback as a continuous loop. The controls work in the same way as the user motions, and you can control playback speed using the “Speed” effect. 
### Configuration
Clicking the “Options” button in the top left corner, settings for configuring Audio Input/Output devices are accessible. 
