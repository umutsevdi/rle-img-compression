<br />
<p align="center">
  <a href="https://github.com/umutsevdi/RLE-Image-Compression">
    <img src="https://cdn.iconscout.com/icon/free/png-512/c-programming-569564.png" alt="Logo" width="80" height="80">
  </a>
  
  <h3 align="center">Run Length Encoding Image Compression</h3>
  
  <p align="center">A C program that can compress and decompress images, display instances of colors and edit them.</p>
  <p align="center"><img src="screenshots/Screenshot from 2021-08-19 11-54-42.png">
</p>

## How to use
* When the program starts, you'll see two avaliable commands. To run a command type it's name.
  
## Compress
* Compresses a `pgm` image and saves it as `sqz` file format. Uses RLE-Image Compression.

  `
    RLE stands for Run Length Encoding. Images commonly have the same information repeated numerous times in a row. Images can have strips of the same pixel color for example. Run Length Compression stores representations of this data in per run format, thereby compressing the data
  `

## Construct
* Reads the given compressed `.sqz` file.
  <p align="center">
  <img src="screenshots/Screenshot from 2021-08-19 11-55-09.png">
  </p>
* Then allows you to choose one of following commands.
  * decode : Reconstructs the `pgm` image.
  * recolor : Chagnnges all instances of given color.
  * edit : `not avaliable` Edits given pixel in the file.
  * histogram : displays instances of each color in the file.


<p align="center">
<img src="screenshots/Screenshot from 2021-08-19 11-55-33.png">
</p>
