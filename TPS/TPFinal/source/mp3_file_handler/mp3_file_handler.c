
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

#define FILE_ARRAY_SIZE 200
#define FILE_NAME_STRING_SIZE	200

/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static MP3Object_t current_objects[FILE_ARRAY_SIZE];
static unsigned int objectsCounter = 0;

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
	objectsCounter = 0;
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


void mp3Files_AddObject(char *path, mp3_object_type_t object_type)
{
	strcpy(current_objects[objectsCounter].path, path);
	current_objects[objectsCounter].index = objectsCounter;
	current_objects[objectsCounter].object_type = object_type;
	objectsCounter++;
}


MP3Object_t mp3Files_GetFirstObject(void)
{
	if (objectsCounter == 0)
	{
		MP3Object_t nullFile = {.object_type = NULL_OBJECT, .index = -1, .path = ""};
		return nullFile;
	}
	return current_objects[0];
}


MP3Object_t mp3Files_GetNextObject(MP3Object_t currentObject)
{
	unsigned int nextObjectIndex = currentObject.index + 1;
	if (nextObjectIndex == objectsCounter)
	{
		nextObjectIndex = 0;
	}
	return current_objects[nextObjectIndex];
}


MP3Object_t mp3Files_GetPreviousObject(MP3Object_t currentObject)
{
	unsigned int previousObjectIndex = currentObject.index - 1;
	if (currentObject.index == 0)
	{
		previousObjectIndex = objectsCounter - 1;
	}
	return current_objects[previousObjectIndex];
}


char* mp3Files_GetObjectName(MP3Object_t object)
{
	char *name;
	name = strrchr(object.path, '/');
	return name;
}


int mp3Files_GetObjectsCounter(void)
{
	return objectsCounter;
}


MP3Object_t mp3Files_ResetObjects(void)
{
	objectsCounter = 0;
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

		char * a = ".";
		a[0] = 1;

		if (strcmp(parentParentDir, a) == 0)
		{
			return mp3Files_GetFirstObject();
		}
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
	char * fn = "..";
	*(newPath+i) = '/';
	strcpy(newPath+i+1, fn);

	// Save the first object as a return to previous directory object
	objectsCounter = 0;

	mp3Files_AddObject(newPath, RETURN_DIR);

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

				mp3Files_AddObject(newPath, object_type);

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
					mp3Files_AddObject(newPath, object_type);
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
    	char * a = ".";
    	a[0] = 1;
    	strcpy(parentDir, a);
    }
}
