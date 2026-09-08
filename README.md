# MP3 Tag Reader and Editor

A command-line utility written in C for viewing and editing basic ID3v2 metadata in MP3 files.

## Purpose

This project reads information stored inside MP3 files, such as the title, artist, album, year, genre, and comments. It can also update these tags.

The project is useful for:

- Correcting incorrect song information.
- Updating basic MP3 metadata.
- Learning C file handling.
- Learning how binary file formats are structured.
- Practicing command-line arguments, pointers, memory allocation, and file operations.

## Features

- View MP3 metadata.
- Edit title, year, artist, album, genre, and comments.
- Read ID3v2 headers and frames directly.
- Use a command-line interface.

## Project Structure

```text
MP3/
├── main.c       # Program entry point and command-line handling
├── view.c       # Reading and displaying MP3 tags
├── view.h       # Declarations for view functions
├── edit.c       # Editing MP3 tags
├── edit.h       # Declarations for edit functions
└── sample.mp3   # Sample MP3 file for testing
```

## Requirements

- GCC or Clang
- macOS, Linux, or another system with a C compiler

## Build

```bash
gcc -Wall -Wextra -Wpedantic main.c view.c edit.c -o a.out
```

## Usage

### View MP3 tags

```bash
./a.out -v sample.mp3
```

### Edit MP3 tags

```bash
./a.out -e <option> "<new value>" sample.mp3
```

| Option | ID3 Frame | Description |
|---|---|---|
| `-t` | `TIT2` | Title |
| `-y` | `TYER` | Year |
| `-a` | `TPE1` | Artist |
| `-A` | `TALB` | Album |
| `-g` | `TCON` | Genre |
| `-c` | `COMM` | Comment |

Examples:

```bash
./a.out -e -t "New Title" sample.mp3
./a.out -e -a "New Artist" sample.mp3
./a.out -e -A "New Album" sample.mp3
./a.out -e -y "2025" sample.mp3
./a.out -e -g "Rock" sample.mp3
./a.out -e -c "New Comment" sample.mp3
```

## How It Works

1. The program receives a command from the terminal.
2. It checks the MP3 filename extension.
3. It reads the ID3v2 header and metadata frames.
4. In view mode, it displays the selected metadata.
5. In edit mode, it writes the updated metadata to a temporary file.
6. The temporary file replaces the original MP3 file.

## Notes

- The program accepts files with a `.mp3` extension.
- The included sample uses the ID3v2.3 format.
- The program is intended for basic ID3v2 metadata.
- Always keep a backup of an MP3 file before editing it.

## Author

Created as a C programming project to practice file handling and MP3 metadata processing.
