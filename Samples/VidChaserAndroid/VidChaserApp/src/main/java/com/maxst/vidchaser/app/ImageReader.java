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
	private int lastIndex;

	ImageReader() {
		currentIndex = 0;
	}

	public void setPath(String path) {
		fileList = new File(path).listFiles();
		Arrays.sort(fileList);

		lastIndex = fileList.length - 1;
	}

	boolean hasNext() {
		if (isRewind) {
			return (currentIndex >= 0);
		} else {
			return (currentIndex < lastIndex);
		}
	}

	byte[] readFrame() {
		byte[] imageData = FileUtil.readImageBytesFromFile(fileList[currentIndex].getAbsolutePath());

		if (isRewind) {
			currentIndex--;
			if (currentIndex < 0) {
				currentIndex = 1;
				isRewind = false;
			}
		} else {
			currentIndex++;
			if (currentIndex >= lastIndex) {
				currentIndex = 0;
			}
		}

		return imageData;
	}

	int getCurrentIndex() {
		return currentIndex;
	}

	void rewind() {
		isRewind = true;
	}

	void reset() {
		if (isRewind) {
			currentIndex = 1;
			isRewind = false;
		} else {
			currentIndex = 0;
		}
	}
}