/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import java.util.ArrayList;

public class ARManager {

	ArrayList<Trackable> trackableArrayList;

	public ARManager() {
		trackableArrayList = new ArrayList<>();
	}

	public void startTracking(Sticker sticker, int imageIndexWhenTouch, int lastImageIndex, int screenX, int screenY, int trackingMethod) {
		for (Trackable trackable : trackableArrayList) {
			if (trackable.sticker == sticker) {
				trackable.start(imageIndexWhenTouch, lastImageIndex, screenX, screenY, trackingMethod);
				break;
			}
		}
	}

	public void updateTransform(int imageIndex) {
		for (Trackable trackable : trackableArrayList) {
			trackable.updateTransform(imageIndex);
		}
	}

	public void drawSticker(int imageIndex) {
		for (Trackable trackable : trackableArrayList) {
			trackable.draw(imageIndex);
		}
	}

	public void drawSticker() {
		for (Trackable trackable : trackableArrayList) {
			trackable.draw();
		}
	}

	public void stopTracking(Sticker sticker) {
		for (Trackable trackable : trackableArrayList) {
			if (trackable.sticker == sticker) {
				trackable.stop();
				break;
			}
		}
	}

	public void addSticker(Sticker sticker) {
		Trackable trackable = new Trackable(sticker);
		trackableArrayList.add(trackable);
	}

	public void removeSticker(Sticker sticker) {
		for (Trackable trackable : trackableArrayList) {
			if (trackable.sticker == sticker) {
				trackableArrayList.remove(trackable);
				break;
			}
		}
	}

	public Sticker getTouchedSticker(int glX, int glY) {

		for (int i = trackableArrayList.size() - 1; 0 <= i; i--) {
			if (trackableArrayList.get(i).sticker.isTouched(glX, glY)) {
				return trackableArrayList.get(i).sticker;
			}
		}

		return null;
	}

	public void clear() {
		trackableArrayList.clear();
	}

	public void deativateAllTrackingPoint() {
		for (Trackable trackable : trackableArrayList) {
			trackable.deactivateTrackingPoint();
		}
	}
}
