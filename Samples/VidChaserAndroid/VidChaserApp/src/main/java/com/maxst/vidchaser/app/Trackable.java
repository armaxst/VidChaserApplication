/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.opengl.Matrix;

import com.maxst.vidchaser.ProjectionMatrix;
import com.maxst.vidchaser.VidChaserAPI;

import java.util.HashMap;

public class Trackable {

	private int trackableId;
	private int imageCoordX, imageCoordY;
	private HashMap<Integer, float[]> transformRecord;
	private int imageIndexWhenTouch;
	private int lastIndex;
	Sticker sticker;

	public Trackable() {
		trackableId = -1;
		imageCoordX = 0;
		imageCoordY = 0;
		transformRecord = new HashMap<>();
		imageIndexWhenTouch = -1;
		lastIndex = -1;
	}

	public Trackable(Sticker sticker) {
		this();
		this.sticker = sticker;
	}

	public void start(int imageIndexWhenTouch, int lastIndex, int screenX, int screenY, int trackingMethod) {
		this.imageIndexWhenTouch = imageIndexWhenTouch;
		this.lastIndex = lastIndex;

		int[] tempTrackableId = new int[1];
		int [] imageCoords = new int[2];
		VidChaserAPI.getImageCoordFromScreenCoord(screenX, screenY, imageCoords);
		VidChaserAPI.addTrackingPoint(imageCoords[0], imageCoords[1], tempTrackableId, trackingMethod);
		VidChaserAPI.activateTrackingPoint(imageCoordX, imageCoordY, tempTrackableId[0]);

		this.imageCoordX = imageCoords[0];
		this.imageCoordY = imageCoords[1];
		this.trackableId = tempTrackableId[0];
	}

	public void draw(int currentImageIndex) {
		if (currentImageIndex == lastIndex) {
			VidChaserAPI.deactivateTrackingPoint(trackableId);
			trackableId = -1;
			imageIndexWhenTouch = -1;
			lastIndex = -1;
		}

		if (currentImageIndex == imageIndexWhenTouch) {
			VidChaserAPI.activateTrackingPoint(imageCoordX, imageCoordY, trackableId);
		}

		if (trackableId != -1) {
			float[] resultTransform3x3 = new float[9];
			float[] glTransform4x4 = new float[16];
			int[] processTime = new int[1];

			int result = VidChaserAPI.getTrackingResult(resultTransform3x3, trackableId, processTime);

			if (result == 0) {
				VidChaserAPI.getTransformMatrix44F(ProjectionMatrix.getInstance().getImageWidth(), ProjectionMatrix.getInstance().getImageHeight(),
						resultTransform3x3, glTransform4x4);
				transformRecord.put(currentImageIndex, glTransform4x4);
			}

			if (imageIndexWhenTouch < currentImageIndex) {
				imageIndexWhenTouch = currentImageIndex;
			}
		}

		if (currentImageIndex == 0) {
			VidChaserAPI.deactivateTrackingPoint(trackableId);
		}

		float[] transform = new float[16];
		Matrix.setIdentityM(transform, 0);
		if (this.transformRecord.containsKey(currentImageIndex)) {
			transform = transformRecord.get(currentImageIndex);
		}
		sticker.draw(transform);
	}

	public void stop() {
		VidChaserAPI.deactivateTrackingPoint(trackableId);
		trackableId = -1;
		imageIndexWhenTouch = -1;
		lastIndex = -1;
		transformRecord.clear();
	}
}