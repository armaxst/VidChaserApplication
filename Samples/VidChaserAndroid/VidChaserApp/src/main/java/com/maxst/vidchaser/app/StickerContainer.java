/*
 * Copyright 2016 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.vidchaser.app;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;

import java.util.List;

public class StickerContainer extends RelativeLayout implements View.OnClickListener {

	private static final String TAG = StickerContainer.class.getSimpleName();

	private boolean textureInitDone = false;
	private List<Texture> stickerList;
	private MotionEvent event;

	public StickerContainer(Context context, AttributeSet attrs) {
		super(context, attrs);

		Button closeButton = new Button(context);
		closeButton.setText("Close");
		closeButton.setOnClickListener(this);
		RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
		layoutParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, RelativeLayout.TRUE);
		addView(closeButton, layoutParams);
	}

	@Override
	public void onClick(View v) {
		setVisibility(View.GONE);
	}

	void setStickerData(List<Texture> stickers) {
		this.stickerList = stickers;
	}

	public static final int MAX_COLUMN_COUNT = 5;
	public static final float STICKER_WIDTH_RATIO = 0.15f;

	private void addSticker() {
		int width = getWidth();
		int height = getHeight();

		int stickerWidth = (int) (width * STICKER_WIDTH_RATIO);
		int stickerHeight = stickerWidth;

		for (int i = 0; i < stickerList.size(); i++) {
			Texture texture = stickerList.get(i);
			int[] data = new int[texture.width * texture.height];

			for (int p = 0; p < texture.width * texture.height; ++p) {
				data[p] = ((texture.data[p * 4 + 3] << 24) & 0xff000000) |
						((texture.data[p * 4] << 16) & 0x00ff0000) |
						((texture.data[p * 4 + 1] << 8) & 0x0000ff00) |
						((texture.data[p * 4 + 2]) & 0x000000ff);
			}

			Bitmap bitmap = Bitmap.createBitmap(data, 0, texture.width, texture.width, texture.height, Bitmap.Config.ARGB_8888);
			ImageView imageView = new ImageView(getContext());
			imageView.setImageBitmap(bitmap);
			imageView.setTag(texture);
			imageView.setOnTouchListener(new OnTouchListener() {
				@Override
				public boolean onTouch(View v, MotionEvent event) {
					switch (event.getAction()) {
						case MotionEvent.ACTION_DOWN:
							break;

						case MotionEvent.ACTION_MOVE:
							break;

						case MotionEvent.ACTION_UP:
							StickerContainer.this.event = event;
					}
					return false;
				}
			});

			imageView.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
					if (stickerSelectedListener != null) {
						stickerSelectedListener.onStickerSelected((Texture)v.getTag(), v.getWidth(), event);
						setVisibility(View.GONE);
					}
				}
			});

			RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(stickerWidth, stickerHeight);
			layoutParams.topMargin = 200;
			layoutParams.leftMargin = i * stickerWidth + 30;
			addView(imageView, layoutParams);
		}
	}

	public void show() {
		postDelayed(new Runnable() {
			@Override
			public void run() {
				if (!textureInitDone) {
					addSticker();
				}
			}
		}, 50);

		setVisibility(View.VISIBLE);
	}

	interface OnStickerSelectedListener {
		void onStickerSelected(Texture texture, int size, MotionEvent motionEvent);
	}

	private OnStickerSelectedListener stickerSelectedListener;

	public void setOnStickerSelected(OnStickerSelectedListener listener) {
		stickerSelectedListener = listener;
	}
}
