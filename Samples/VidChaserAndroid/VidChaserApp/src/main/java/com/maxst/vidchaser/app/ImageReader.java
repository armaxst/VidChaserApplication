/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import java.io.File;
import java.util.Arrays;

class ImageReader {
	private File fileList[];
	private int currentIndex;
	private long timestamp;
	private boolean isRewind = false;
	private int lastIndex;

	ImageReader() {
		currentIndex = -1;
		timestamp = -1;
	}

	public void setPath(String path) {
		fileList = new File(path).listFiles();
		Arrays.sort(fileList);

		lastIndex = fileList.length - 1;
	}

	boolean hasNext() {
		if (isRewind) {
			return (currentIndex > 0);
		} else {
			return (currentIndex < lastIndex);
		}
	}

	boolean isLast() {
		return (currentIndex == lastIndex);
	}

	byte[] readFrame() {
		if (currentIndex == -1) {
			currentIndex = 0;
			timestamp = 0;
		} else if (isRewind) {
			currentIndex--;
			timestamp -= 33;
		} else {
			currentIndex++;
			timestamp += 33;
		}
		return FileUtil.readImageBytesFromFile(fileList[currentIndex].getAbsolutePath());
	}

	int getCurrentIndex() {
		return currentIndex;
	}

	public void rewind() {
		this.isRewind = true;
	}

	long getTimestamp() {
		return timestamp;
	}

	void reset() {
		currentIndex = -1;
		timestamp = -1;
		isRewind = false;
	}
}