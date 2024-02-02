
/***************************************************************************/ /**
  @file     mp3_file_handler.c
  @brief    MP3 File Handler functions
  @author   Grupo 5 - Lab de Micros
 ******************************************************************************/

#include "mp3_file_handler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ff.h"

#define FILE_ARRAY_SIZE 		100
#define FILE_NAME_STRING_SIZE	200

/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static MP3Object_t current_directories[FILE_ARRAY_SIZE];
static unsigned int directoriesCounter = 0;

static MP3Object_t current_songs[FILE_ARRAY_SIZE];
static unsigned int songsCounter = 0;

static MP3Object_t playing_songs[FILE_ARRAY_SIZE];
static unsigned int playingSongsCounter = 0;



/*
 *
 */
static void mp3Files_list_dir(char * path);

/*
 *
 */
static void getParentDirectory(char* filePath, char* parentDir);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void mp3Files_Init(void)
{
	// Set path in 0
	directoriesCounter = 0;
	songsCounter = 0;
	playingSongsCounter = 0;

	char path[FILE_NAME_STRING_SIZE] = {0};
	mp3Files_list_dir("");
}


bool mp3Files_isMp3File(char *path)
{
	char *extension;
	if ((extension = strrchr(path, '.')) != NULL)
	{
		if (strcmp(extension, ".mp3") == 0)
		{
			return true;
		}
	}
	return false;
}


void mp3Files_AddDirectory(char *path, mp3_object_type_t object_type)
{
	strcpy(current_directories[directoriesCounter].path, path);
	current_directories[directoriesCounter].index = directoriesCounter;
	current_directories[directoriesCounter].object_type = object_type;
	directoriesCounter++;
}


void mp3Files_AddSong(char *path, mp3_object_type_t object_type)
{
	strcpy(current_songs[songsCounter].path, path);
	current_songs[songsCounter].index = songsCounter;
	current_songs[songsCounter].object_type = object_type;
	songsCounter++;
}


void mp3Files_AddPlayingSongs(char *path, mp3_object_type_t object_type)
{
	strcpy(playing_songs[playingSongsCounter].path, path);
	playing_songs[playingSongsCounter].index = playingSongsCounter;
	playing_songs[playingSongsCounter].object_type = object_type;
	playingSongsCounter++;
}


MP3Object_t mp3Files_GetFirstObject(void)
{
	if (directoriesCounter == 0 && songsCounter == 0)
	{
		// If there are no objects, including RETURN_DIR, return a NULL_OBJECT
		MP3Object_t nullFile = {.object_type = NULL_OBJECT, .index = -1, .path = ""};
		return nullFile;
	}
	else if (directoriesCounter == 1 && songsCounter == 0)
	{
		// Return folder to return to previous folder if there is only one folder
		return current_directories[0];
	}
	else if(directoriesCounter > 1)
	{
		// If there are more than one folder, then the second one is the first directory
		return current_directories[1];
	}
	else if(songsCounter > 0)
	{
		// If there are songs, and no first directory, return the first song
		return current_songs[0];
	}

	// Return the first object, that is not to return to previous folder
	return current_directories[1];
}


void updatePlayingSongs(void)
{
	uint32_t i;
	for(i = 0; i < songsCounter; i++)
	{
		playing_songs[i] == current_songs[i];
	}
}


MP3Object_t mp3Files_GetNextObject(MP3Object_t currentObject)
{
	unsigned int nextObjectIndex = currentObject.index + 1;

	if(currentObject.object_type == DIRECTORY || currentObject.object_type == RETURN_DIR)
	{
		// If the current Object is a directory
		if (nextObjectIndex == directoriesCounter)
		{
			// If the next index is out of range, go to the songs list
			nextObjectIndex = 0;
			if (nextObjectIndex == songsCounter)
			{
				// If the next index is out of range, return the first directory (no mp3 files)
				return current_directories[nextObjectIndex];
			}
			else
			{
				// Return the first song
				return current_songs[nextObjectIndex];
			}
		}
		else
		{
			// If the next index is on range, return the next directory
			return current_directories[nextObjectIndex];
		}

	}
	else
	{
		// If the current Object is a song
		if (nextObjectIndex == songsCounter)
		{
			// If the next index is out of range, go to the directories list
			nextObjectIndex = 0;
			return current_directories[nextObjectIndex];
		}
		else
		{
			// If the next index is on range, return the next song
			return current_songs[nextObjectIndex];
		}
	}
}


MP3Object_t mp3Files_GetNextMP3File(MP3Object_t currentObject)
{
	unsigned int nextObjectIndex = currentObject.index + 1;
	if (nextObjectIndex == playingSongsCounter)
	{
		nextObjectIndex = 0;
	}
	return playing_songs[nextObjectIndex];
}

/*
MP3Object_t mp3Files_GetNextMP3File2(MP3Object_t currentObject)
{
	uint32_t i;
	unsigned int nextObjectIndex;

	for (i = 1; i < objectsCounter; i++)
	{
		nextObjectIndex = (currentObject.index + i) % objectsCounter;

		if (current_objects[nextObjectIndex].object_type == MP3_FILE)
			return current_objects[nextObjectIndex];
	}

	// If no other MP3_FILES
	return currentObject;
}*/


MP3Object_t mp3Files_GetPreviousObject(MP3Object_t currentObject)
{
	unsigned int previousObjectIndex = currentObject.index - 1;

	if(currentObject.object_type == DIRECTORY || currentObject.object_type == RETURN_DIR)
	{
		// If the current Object is a directory
		if (currentObject.index == 0)
		{
			// If the next index is out of range, go to the songs list
			previousObjectIndex = songsCounter;
			if (previousObjectIndex == 0)
			{
				// If the next index is out of range, return the first directory (no mp3 files)
				return current_directories[directoriesCounter-1]; //TODO: SI FALLA, ES POR EL -1
			}
			else
			{
				// Return the first song
				return current_songs[songsCounter-1];
			}
		}
		else
		{
			// If the next index is on range, return the next directory
			return current_directories[previousObjectIndex];
		}

	}
	else
	{
		if (currentObject.index == 0)
		{
			// If the next index is out of range, go to the directories
			return current_directories[directoriesCounter-1];
		}
		else
		{
			// If the next index is on range, return the next directory
			return current_songs[previousObjectIndex];
		}
	}
}


MP3Object_t mp3Files_GetPreviousMP3File(MP3Object_t currentObject)
{
	unsigned int previousObjectIndex = currentObject.index - 1;
	if (currentObject.index == 0)
	{
		previousObjectIndex = playingSongsCounter-1;
	}
	return playing_songs[previousObjectIndex];
}


/*
MP3Object_t mp3Files_GetPreviousMP3File2(MP3Object_t currentObject)
{
	uint32_t i;
	uint32_t nextObjectIndex;

	for (i = 1; i < objectsCounter; i++)
	{
		nextObjectIndex = (currentObject.index + objectsCounter - i) % objectsCounter;

		if (current_objects[nextObjectIndex].object_type == MP3_FILE)
			return current_objects[nextObjectIndex];
	}

	// If no other MP3_FILES
	if (currentObject.object_type != MP3_FILE)
	{
		MP3Object_t nullFile = {.object_type = NULL_OBJECT, .index = -1, .path = ""};
		return nullFile;
	}
	else
	{
		// If the current object is an MP3_FILE, return it
		return currentObject;
	}
}*/


char* mp3Files_GetObjectName(MP3Object_t object)
{
	char *name;
	name = strrchr(object.path, '/');
	return name;
}


int mp3Files_GetObjectsCounter(void)
{
	return directoriesCounter;
}


MP3Object_t mp3Files_ResetObjects(void)
{
	directoriesCounter = 0;
	songsCounter = 0;
	return mp3Files_GetFirstObject();
}


MP3Object_t mp3Files_Enter_Dir(MP3Object_t object)
{
	if (object.object_type == DIRECTORY)
	{
		mp3Files_list_dir(object.path);
		return mp3Files_GetFirstObject();
	}
}


MP3Object_t mp3Files_Exit_Dir(MP3Object_t object)
{
	if (object.object_type == RETURN_DIR)
	{
		char parentDir[FILE_NAME_STRING_SIZE];
		char parentParentDir[FILE_NAME_STRING_SIZE];

		getParentDirectory(object.path, parentDir);
		getParentDirectory(parentDir, parentParentDir);

		char * a = "";

		mp3Files_list_dir(parentParentDir);

		return mp3Files_GetFirstObject();
	}
}


void getCurrentDirectory(char* buff, unsigned int len)
{
	f_getcwd(buff, len);
}

/*************************************************************************************
 * 		LOCAL FUNCTIONS DECLARATIONS
 ************************************************************************************/
static void mp3Files_list_dir(char * path)
{
	FRESULT res;
	DIR dir;
	FILINFO fno;
	int nfile, ndir;

	unsigned int i = strlen(path);
	char newPath[FILE_NAME_STRING_SIZE] = {0};

	strcpy(newPath, path);
	char * fn = "...";
	*(newPath+i) = '/';
	strcpy(newPath+i+1, fn);

	// Save the first object as a return to previous directory object
	directoriesCounter = 0;
	songsCounter = 0;

	mp3Files_AddDirectory(newPath, RETURN_DIR);

	// Save the remaining objects

	res = f_opendir(&dir, path);                       /* Open the directory */
	if (res == FR_OK)
	{
		nfile = ndir = 0;
		for (;;)
		{
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0) break;  /* Error or end of dir */

			// !(fno.fattrib & (AM_HID | AM_SYS))
			// Checks that the file is not a hidden file or system file
			if ((!(fno.fattrib & (AM_HID | AM_SYS))) && (fno.fattrib & AM_DIR))
			{            /* Directory */
				// Complete the path of the new directory
				char * fn = fno.fname;
				*(newPath+i) = '/';
				strcpy(newPath+i+1, fn);

				mp3_object_type_t object_type = DIRECTORY;

				mp3Files_AddDirectory(newPath, object_type);

			}
			else if (!(fno.fattrib & (AM_HID | AM_SYS)))
			{                               /* File */
				// Complete the path of the new file
				char * fn = fno.fname;
				*(newPath+i) = '/';
				strcpy(newPath+i+1, fn);

				mp3_object_type_t object_type = MP3_FILE;

				if(mp3Files_isMp3File(newPath))
				{
					mp3Files_AddSong(newPath, object_type);
				}
			}
		}
		f_closedir(&dir);
	}
	else
	{
		printf("Failed to open \"%s\". (%u)\n", path, res);
	}
}


static void getParentDirectory(char* filePath, char* parentDir)
{
    // Copy the input path to the result path
    strncpy(parentDir, filePath, FILE_NAME_STRING_SIZE);

    // Find the last occurrence of the directory separator '/'
    char* lastSlash = strrchr(parentDir, '/');

    if (lastSlash != NULL) {
        // Terminate the string at the last slash to remove the filename
        *lastSlash = '\0';
    } else {
        // No directory separator found, assuming the file is in the current directory
    	char * a = "";
    	strcpy(parentDir, a);
    }
}
