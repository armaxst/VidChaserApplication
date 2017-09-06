package com.maxst.vidchaser.app;


public class FileInfo {

	int index;
	int seekPosition;
	String fileName;

	FileInfo(int index, int seekPosition, String fileName) {
		this.index = index;
		this.seekPosition = seekPosition;
		this.fileName = fileName;
	}

	int getIndex() {
		return index;
	}

	int getSeekPosition() {
		return seekPosition;
	}

	String getFileName() {
		return fileName;
	}
}
