## Get Relief! 
**Relief Map Generator**

Get Relief! Is a new Relaxed Cone Step Map (RCSM) and Height Map Generator for Unreal Engine, implemented directly inside the engine.

**Convert any flat texture to one with depth, all you need is a heightmap, the tool will take care of the rest.**

This plugin aims to simplify and demystify different Relief Mapping techniques for materials. The engine supports Parallax Occlusion Mapping (POM) but to get good quality results might require a cost-prohibitive number of samples. 

RCSM (described by Nvidia GPU Gems 3 : Chapter 18) provides a method for creating a helper relief map texture that can reduce the required number of samples for appropriate occlusion substantially. This technology has been used in many recent AAA games, but often is out of reach of Indie devs.

This plugin wraps up the efficiency and behaviors of RCSM into an all in one package for generating these helper textures, but with a focus on production ready workflows.

### Features:
* Right-Click Menu on texture assets to generate RCSM or Height maps.
* Modify height-maps at interactive framerates with novel cross-section preview.
* Progress Bars, Auto-save, channel previews and more UX features.
* Variable source height-map channels, variable 
* Generation Settings saved with the outputted texture files seamlessly.
* AAA quality, versatile material functions for relief mapping on surfaces and decals:
* Relaxed Cone Step Mapping
* Improved Parallax Occlusion Mapping (Higher quality with few samples.)
* Parallax Occlusion Mapping: Epic Version (Standard Engine POM, but with the graphed cleaned up for readability and modability)
* Transform World Vector to Decal Tangent Vector.
* Example Decal and Megascans materials to learn from.


# Documentation:
https://github.com/Ryan-DowlingSoka/ReliefMapping/wiki/Get-Relief!-Wiki

# Licensing:
This software is available for purchase through the Epic Games Store Marketplace.

For alternative licensing, contact me directly ryan[at]dowlingsoka.com
