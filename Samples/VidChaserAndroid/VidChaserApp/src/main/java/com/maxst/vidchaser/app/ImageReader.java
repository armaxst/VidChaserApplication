/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

class ImageReader {
	private List<FileInfo> fileInfoList = new ArrayList<>();
	private int currentIndex;
	private boolean isRewind = false;

	ImageReader() {
		currentIndex = 0;
	}

	boolean setPath(String path) {
		File [] fileList = new File(path).listFiles();
		Arrays.sort( fileList, new Comparator() {
			public int compare(Object o1, Object o2) {

				if (((File)o1).lastModified() > ((File)o2).lastModified()) {
					return 1;
				} else if (((File)o1).lastModified() < ((File)o2).lastModified()) {
					return -1;
				} else {
					return 0;
				}
			}
		});

		for (int i = 0; i < fileList.length; i++) {
			String fileName = fileList[i].getAbsolutePath();
			String fileNameWithoutExtension = fileName.substring(fileName.lastIndexOf("/") + 1, fileName.lastIndexOf("."));
			fileInfoList.add(new FileInfo(i, Integer.valueOf(fileNameWithoutExtension), fileName));
		}

		return (fileInfoList != null && fileInfoList.size() > 0);
	}

	boolean hasNext() {
		if (isRewind) {
			return (currentIndex >= 0);
		} else {
			return (currentIndex < fileInfoList.size());
		}
	}

	byte[] readFrame() {
		return VidChaserUtil.readImageBytesFromFile(fileInfoList.get(currentIndex).getFileName());
	}

	void updateIndex() {
		if (isRewind) {
			currentIndex--;
		} else {
			currentIndex++;
		}
	}

	int getCurrentIndex() {
		return currentIndex;
	}

	void setCurrentIndex(int index) {
		currentIndex = index;
	}

	int findNearestIndexByPosition(int seekPosition) {
		for (int i = 0; i < fileInfoList.size() - 1; i++) {
			if (seekPosition > fileInfoList.get(i).getSeekPosition() && seekPosition <= fileInfoList.get(i + 1).getSeekPosition()) {
				return fileInfoList.get(i + 1).getIndex();
			}
		}

		return -1;
	}

	int getLastIndex() {
		return fileInfoList.size() - 1;
	}

	void rewind() {
		isRewind = true;
		//nextIndex -= 1;
	}

	void reset() {
		if (isRewind) {
			isRewind = false;
		}

		currentIndex = 0;
	}
}