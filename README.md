# calenhad
Calenhad is GIS and terrain creation for imaginary worlds.

This allows a chain of things that look like libnoise modules (but are actually algorithms implemented on the GPU) to be created and edited and used to generate world maps. For a technical summary and acknowledgements, please see about.txt.

To use: you drag modules from the palette onto the desktop and connect the output of one to an input of another to create connections between them. Double click on the noise module icon on the desktop to bring up a window allowing editing of parameters. On the parameter tab, enter values as numbers or as mathematical expressions; if you want a variable, create it by opening the variables dialog on the Edit menu. Click on th globe icon to bring up the interactive globe view. Save and load pipelines from the File menu; the XML format it produces should be human-editable. Right-click on the globe view for a context menu supporting some preferences.
  
This has not been tested rigorously at this point and probably has plenty of opportunities to segfault at no notice. It is very much experimental and a work in progress and there is still a lot to do. Any use made of it (if you can make any at all) is at own risk. 

Compile using cmake in the usual way. These are dependencies:

  - exprtk: parsing and calculating mathematical formulae entered as text
  - QWT: some of the widgets, such as the compass / navigator that appears on the map
  - GeographicLib: geodesic calculations on the geoid (we always use a sphere)
  - OpenGL: to compile and run the Übershader and display maps and globes from its output
  - QuaZip and its engine zlib: to package and unpackage the project files so they can be distributed as a single archive and dependencies managed within it.

This work, such as it is, is (c) by Martin Bucknall (chateauferret@yahoo.co.uk) and is made available as is and without warranty of any kind. Comments are welcome, spam is not.
