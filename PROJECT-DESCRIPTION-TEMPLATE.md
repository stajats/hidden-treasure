# Hidden treasure

mi23046 - Tadija Matić

Project developed for the Computer Graphics course at the [Faculty of Mathematics](http://www.matf.bg.ac.rs/) (University of Belgrade).


The project features a small island with a hidden treasure, in a sunrise setting, surrounded by rocks and a ship. It implements three different types of lighting, point shadows, HDR bloom, and an animated water surface. A special lighting event can be triggered, causing the skull to emit an intense glow for a few seconds. The engine has also been extended to support post-processing effects, point shadows, and bloom.

## Controls


- **W** → Move forward
- **A** → Move left
- **S** → Move backward
- **D** → Move right

- **E** → Trigger the skull lighting event
- **Q** → Toggle the GUI

- **Esc** → Exit the program

## Features

* Blinn–Phong lighting
* Animated water
* Adjustable lighting colors through GUI sliders
* Toggleable lighting and visual effects
* Post-processing support
* Point shadows
* HDR bloom

### Fundamental:

* [x] Model with lighting
* [x] Two types of lighting with customizable colors and movement through the GUI or actions
* [x] {ACTION_X} --- AFTER_M_SECONDS ---> Triggers ---> {EVENT_A} --- AFTER_N_SECONDS ---> Triggers ---> {EVENT_B}

### Group A:

* [ ] Framebuffers with post-processing
* [ ] Off-screen Anti-Aliasing
* [ ] Parallax Mapping
* [x] HDR bloom

### Group B:

* [ ] Deferred Shading
* [x] Point Shadows
* [ ] SSAO

### Engine improvement:

The engine has been extended to support post-processing, point shadows, and HDR bloom.

## Models:

* ship - https://poly.pizza/m/mEQj2wZ3GC
* boat - https://poly.pizza/m/W2vMzztgIi
* island - Island by Poly by Google [CC-BY] (https://creativecommons.org/licenses/by/3.0/) via Poly Pizza (https://poly.pizza/m/bzLVwG4AzvA)
* chest - Chest with Gold by Quaternius (https://poly.pizza/m/haqf9qoiOG)
* rock - Rock by Quaternius (https://poly.pizza/m/4MUaQTcDdc)
* big_rock - Rocks by Quaternius (https://poly.pizza/m/Gl3pcrdnpW)
* small_rock - Rock by Quaternius (https://poly.pizza/m/RtLRqYjfMs)
* lantern - Lantern by Kay Lousberg (https://poly.pizza/m/CtHBJ1ufeW)
* flame - Flame Decal by Quaternius (https://poly.pizza/m/8DEKYKfH2h)

Textures for all models are included with their respective model downloads.

## Textures

* sunrise - https://freestylized.com/skybox/sky_clouds_12/
* water - https://www.artstation.com/artwork/zAX5bL

## Resources used

* LearnOpenGL by JoeyDeVries
  https://github.com/JoeyDeVries/LearnOpenGL

* project_base by spaske00
  https://github.com/matf-racunarska-grafika/matf-rg-project-2024
