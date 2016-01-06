Introduction

This project was started with the goal of creating an open map, multiplayer, 2D space explorer. The first release candidate will aim to have at least a local server possible, which will allow dynamic multiplayer play. Eventually, it would be great to have an online server to facilitate this. This project is currently using C++ and two C libraries, the Allegro games library and the Enet UDP networking library.

Setup

If you want to develop for this project, you will need copies of the above libraries installed and will need to configure your project to link to these libraries. If you are using Visual Studio, you can simply put the libraries in "C:/allegro" and "C:/enet" respectively and a clone of this repo should already be linked correctly (I hope!). [Here] (https://wiki.allegro.cc/index.php?title=Windows,_Visual_Studio_2010_and_Allegro_5) is a link to a tutorial to set up VS with the [Allegro 5 windows binary] (https://www.allegro.cc/files/), and [Enet] (http://enet.bespin.org/Downloads.html) can linked in the same way.
