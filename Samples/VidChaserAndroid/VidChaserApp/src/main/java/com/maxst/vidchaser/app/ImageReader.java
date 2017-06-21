/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import java.io.File;
import java.util.Arrays;

class ImageReader {
	private File fileList[];
	private int currentIndex;
	private boolean isRewind = false;

	ImageReader() {
		currentIndex = 0;
	}

	boolean setPath(String path) {
		fileList = new File(path).listFiles();
		Arrays.sort(fileList);
		return (fileList != null && fileList.length > 0);
	}

	boolean hasNext() {
		if (isRewind) {
			return (currentIndex >= 0);
		} else {
			return (currentIndex < fileList.length);
		}
	}

	byte[] readFrame() {
		byte[] imageData = VidChaserUtil.readImageBytesFromFile(fileList[currentIndex].getAbsolutePath());

		if (isRewind) {
			currentIndex--;
		} else {
			currentIndex++;
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
	}

	void reset() {
		if (isRewind) {
			isRewind = false;
		}

		currentIndex = 0;
	}
}