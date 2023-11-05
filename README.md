# mp3WAlbumCover
Can convert almost any music file (I have tested webm and mp3) and image file (I have tested webp and jpg) to mp3 with album cover.
Can be used if yt-dlg fails to do the same job. Remember that the audio and image file needs to have same filename.
It automatically finds the correct image and igonre any file that doesnt have audio or image pair.

commands: 

make

./main [path] [-o outputFolder]

the outputFolder is optional, default is path/mp3_output

so here is example
./main /path/to/folder -o mp3_output

this code will save the result to /path/to/folder/mp3_output
