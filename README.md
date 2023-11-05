# mp3WAlbumCover
Can convert almost any music file (I have tested webm and mp3) and image file (I have tested webp and jpg) to mp3 with album cover.
Can be used if yt-dlg fails to do the same job. Remember that the audio and image file needs to have same filename.
It automatically finds the correct image and igonre any file that doesnt have audio or image pair.

commands: 

make

./main /path/to/the/files

It will output the files to folder /path/to/the/files/mp3_output/. You have to create this folder beforehand. The files will have the same name as the audio, but in mp3 format
