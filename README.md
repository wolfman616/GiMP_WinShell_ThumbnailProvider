# GiMP_WinShell_ThumbnailProvider
Shell Extension handler for .XCF Gimp files in Windows.

<img width="640" alt="fgfg" src="https://github.com/wolfman616/GiMP_WinShell_ThumbnailProvider/assets/62726599/adb0c117-5cb7-465c-9c06-732b3fe53cbe">

Curretly one of the most problematic formats yet to be encountered with regards to automatically generating thumbnail previews in the windows shell.
In deciding how best to proceed in the automation logical chain have chosen to use Imagemagick to render the Xcf files and deal with conversion to PNG -> JFIF then Autohotkey takes the image file and converts it to a base64 Text file whilst disposing of the thumbnail image files. the Base64 then proceeds to be ingested by the ImageFactory making use of IInitializeWithFile, it then disposes of the foementioned Text file(50% of the time as of writing this because the automatic detection of files and automatically deleting them seems to require a bit more precision and practise when issued from the c++ command-land). 

I just freeballed re-writing the ThumbnailProvider class from scratch, as I was unable to find a single working handler for this file type which worked with and type of reliability. There is a lot of refinements to take place still. this is a loose and rugged proof of concept, and may assist someone in pursuite of handling the previous.

*Notable requirement of AHK script to help with the numerous files which need to be automated. IDeally this would all be taking place in memory within the handler in a methodical logical and sane manner but I wasnt immediately able to implement anything close to this, having spent about 40 hours up to this point.

I am still refining the ImageMagick command string to obtain the most desirable dquality to size trade off, which is proving un-trivial also.

todo:
In future will invlude NQ7 neural net indexing to the png stage of the chain to attempt to optimize each thumbnail renditions string length / filestructure footprint in the windows thumbcache.


<img width="748" alt="Clipboarder 2023 07 06-024" src="https://github.com/wolfman616/GiMP_WinShell_ThumbnailProvider/assets/62726599/f89136ac-1b6d-48f6-8156-f91bcf1da228">

<img width="560" alt="Clipboarder 2023 07 06-020" src="https://github.com/wolfman616/GiMP_WinShell_ThumbnailProvider/assets/62726599/0040d80d-3d7c-438a-ae59-413dac5cc713">
