/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import java.io.File;
import java.util.Arrays;

class ImageReader {
	private File fileList[];
	private int currentIndex;
	private int nextIndex;
	private boolean isRewind = false;

	ImageReader() {
		currentIndex = 0;
		nextIndex = 0;
	}

	boolean setPath(String path) {
		fileList = new File(path).listFiles();
		Arrays.sort(fileList);
		return (fileList != null && fileList.length > 0);
	}

	boolean hasNext() {
		if (isRewind) {
			return (nextIndex >= 0);
		} else {
			return (nextIndex < fileList.length);
		}
	}

	byte[] readFrame() {
		currentIndex = nextIndex;

		byte[] imageData = VidChaserUtil.readImageBytesFromFile(fileList[currentIndex].getAbsolutePath());

		if (isRewind) {
			nextIndex--;
		} else {
			nextIndex++;
		}

		return imageData;
	}

	int getCurrentIndex() {
		return currentIndex;
	}

	int getLastIndex() {
		return fileList.length - 1;
	}

	void rewind() {
		isRewind = true;
		nextIndex -= 1;
	}

	void reset() {
		if (isRewind) {
			isRewind = false;
		}

		currentIndex = 0;
		nextIndex = 0;
	}
}