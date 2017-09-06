/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.opengl.Matrix;
import android.util.Log;

import com.maxst.vidchaser.ProjectionMatrix;
import com.maxst.vidchaser.VidChaserAPI;

import java.util.HashMap;
import java.util.Locale;

public class Trackable {

	private static final String TAG = Trackable.class.getSimpleName();

	private int trackableId = -1;
	private int imageCoordX, imageCoordY;
	private HashMap<Integer, float[]> transformRecord;
	private int endIndex;
	private int lastTrackingIndex;
	Sticker sticker;
	private boolean keepTracking = false;
	private boolean trackingEnabled = false;
	private float[] identityMatrix = new float[16];

	public Trackable() {
		transformRecord = new HashMap<>();
		Matrix.setIdentityM(identityMatrix, 0);
	}

	public Trackable(Sticker sticker) {
		this();
		this.sticker = sticker;
	}

	public void start(int imageIndexWhenTouch, int endIndex, int screenX, int screenY, int trackingMethod) {
		this.lastTrackingIndex = imageIndexWhenTouch;

		int[] tempTrackableId = new int[1];
		int[] imageCoords = new int[2];
		VidChaserAPI.getImageCoordFromScreenCoord(screenX, screenY, imageCoords);
		VidChaserAPI.addTrackingPoint(imageCoords[0], imageCoords[1], tempTrackableId, trackingMethod);

		this.imageCoordX = imageCoords[0];
		this.imageCoordY = imageCoords[1];
		this.trackableId = tempTrackableId[0];
		this.endIndex = endIndex;
		keepTracking = true;
		trackingEnabled = true;

		VidChaserAPI.activateTrackingPoint(imageCoordX, imageCoordY, trackableId);

		Log.d(TAG, "Start tracking. Trackable id : " + trackableId);
	}

	public void updateTransform(int currentImageIndex) {
		if (trackableId < 0) {
			sticker.draw(identityMatrix);
			return;
		}

		if (trackingEnabled) {
			if (!keepTracking && currentImageIndex > lastTrackingIndex) {
				VidChaserAPI.activateTrackingPoint(imageCoordX, imageCoordY, trackableId);
				Log.d(TAG, "Restart tracking. Trackable id : " + trackableId);
				keepTracking = true;
			}

			if(keepTracking) {
				float[] resultTransform3x3 = new float[9];
				float[] transposed = new float[9];
				float[] glTransform4x4 = new float[16];
				int[] processTime = new int[1];

				int result = VidChaserAPI.getTrackingResult(resultTransform3x3, trackableId, processTime);

				if (result == 0) {
					VidChaserAPI.transposeMatrix33F(resultTransform3x3, transposed);
					VidChaserAPI.getTransformMatrix44F(ProjectionMatrix.getInstance().getImageWidth(), ProjectionMatrix.getInstance().getImageHeight(),
							transposed, glTransform4x4);
					transformRecord.put(currentImageIndex, glTransform4x4);
				}

				if (lastTrackingIndex < currentImageIndex) {
					lastTrackingIndex = currentImageIndex;
				}
			}

			if(currentImageIndex == endIndex) {
				trackingEnabled = false;
			}
		}
	}

	public void draw(int currentImageIndex) {
		float[] transform = new float[16];
		Matrix.setIdentityM(transform, 0);
		if (this.transformRecord.containsKey(currentImageIndex)) {
			transform = transformRecord.get(currentImageIndex);
		}
		sticker.draw(transform);
	}

	public void stop() {
		VidChaserAPI.removeTrackingPoint(trackableId);
		trackableId = -1;
		lastTrackingIndex = 0;
		transformRecord.clear();
	}

	public void deactivateTrackingPoint() {
		if (trackableId >= 0) {
			VidChaserAPI.deactivateTrackingPoint(trackableId);
		}
		keepTracking = false;
	}

	public Sticker getSticker() {
		return sticker;
	}
}