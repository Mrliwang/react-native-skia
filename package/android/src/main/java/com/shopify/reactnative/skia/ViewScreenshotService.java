package com.shopify.reactnative.skia;

import static android.view.View.VISIBLE;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Outline;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.PixelCopy;
import android.view.SurfaceView;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.ReactContext;
import com.facebook.react.uimanager.ReactShadowNode;
import com.facebook.react.uimanager.UIManagerModule;
import com.facebook.react.views.view.ReactViewGroup;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class ViewScreenshotService {
    private static final long SURFACE_VIEW_READ_PIXELS_TIMEOUT = 5;
    private static final String TAG = "SkiaScreenshot";

    public static Bitmap makeViewScreenshotFromTag(ReactContext context, int tag) {
        UIManagerModule uiManager = context.getNativeModule(UIManagerModule.class);
        View view = null;
        try {
            view = uiManager.resolveView(tag);
        } catch (RuntimeException e) {
            context.handleException(e);
        }
        if (view == null) {
            return null;
        }
        // Measure and get size of view
        int width = view.getWidth();
        int height = view.getHeight();

        if (width <= 0 || height <= 0) {
            return null;
        }

        // The following code is taken from react-native-view-shot to be able to handle and
        // correctly render all kinds of views, also including TextureViews and SurfaceViews
        Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);

        final Paint paint = new Paint();
        paint.setAntiAlias(true);
        paint.setFilterBitmap(true);
        paint.setDither(true);

        // Render the main view and its children
        final Canvas canvas = new Canvas(bitmap);

        // This is the root view we need to offset the content to 0, 0
        canvas.save();
        canvas.translate(-view.getLeft(), -view.getTop());
        // Renders view with child views to canvas
        renderViewToCanvas(canvas, view, paint);
        canvas.restore();

        return bitmap;
    }

    private static void renderViewToCanvas(Canvas canvas, View view, Paint paint) {
        // Apply transformations for the current view
        canvas.save();
        applyTransformations(canvas, view);

        // Draw children if the view has children
        if ((view instanceof ViewGroup)) {
            // Draw children
            ViewGroup group = (ViewGroup) view;
            if (view instanceof ReactViewGroup) {
                try {
                    Class[] cArg = new Class[1];
                    cArg[0] = Canvas.class;
                    Method method = ReactViewGroup.class.getDeclaredMethod("dispatchOverflowDraw", cArg);
                    method.setAccessible(true);
                    method.invoke(view, canvas);
                } catch (Exception e) {
                    Log.e(TAG, "couldn't invoke dispatchOverflowDraw() on ViewGroup");
                }
            }
            // Draw ourselves
            Drawable bg = view.getBackground();
            if (bg != null) {
                canvas.saveLayerAlpha(null, Math.round(view.getAlpha() * 255));
                bg.draw(canvas);
                canvas.restore();
            }




            // Draw children
            for (int i = 0; i < group.getChildCount(); i++) {
                View child = group.getChildAt(i);

                // skip all invisible to user child views
                if (child.getVisibility() != VISIBLE) continue;

                // skip any child that we don't know how to process
                if (child instanceof TextureView) {
                    final TextureView tvChild = (TextureView) child;
                    tvChild.setOpaque(false); // <-- switch off background fill

                    canvas.save();
                    applyTransformations(canvas, child);

                    // TextureView should use bitmaps with matching size,
                    // otherwise content of the TextureView will be scaled to provided bitmap dimensions
                    final Bitmap childBitmapBuffer = tvChild.getBitmap(Bitmap.createBitmap(child.getWidth(), child.getHeight(), Bitmap.Config.ARGB_8888));
                    canvas.drawBitmap(childBitmapBuffer, 0, 0, paint);

                    canvas.restore();

                } else if (child instanceof SurfaceView) {
                    final SurfaceView svChild = (SurfaceView) child;
                    final CountDownLatch latch = new CountDownLatch(1);

                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                        final Bitmap childBitmapBuffer = Bitmap.createBitmap(child.getWidth(), child.getHeight(), Bitmap.Config.ARGB_8888);
                        try {
                            PixelCopy.request(svChild, childBitmapBuffer, copyResult -> {
                                canvas.save();
                                applyTransformations(canvas, child);
                                canvas.drawBitmap(childBitmapBuffer, 0, 0, paint);
                                canvas.restore();
                                latch.countDown();
                            }, new Handler(Looper.getMainLooper()));
                            latch.await(SURFACE_VIEW_READ_PIXELS_TIMEOUT, TimeUnit.SECONDS);
                        } catch (Exception e) {
                            Log.e(TAG, "Cannot PixelCopy for " + svChild, e);
                        }
                    } else {
                        Bitmap cache = svChild.getDrawingCache();
                        if (cache != null) {
                            canvas.save();
                            applyTransformations(canvas, child);
                            canvas.drawBitmap(svChild.getDrawingCache(), 0, 0, paint);
                            canvas.restore();
                        }
                    }
                } else {
                    // Regular views needs to be rendered again to ensure correct z-index
                    // order with texture views and surface views.
                    renderViewToCanvas(canvas, child, paint);
                }
            }
        } else {
            // Draw ourselves
            view.draw(canvas);
        }

        // Restore canvas
        canvas.restore();
    }

    @NonNull
    private static void applyTransformations(final Canvas c, @NonNull final View view) {
        // Get the transformation matrix of the view
        final Matrix matrix = view.getMatrix();

        // Create a new matrix for translation
        final Matrix translateMatrix = new Matrix();
        final float dx = view.getLeft() + view.getPaddingLeft();
        final float dy = view.getTop() + view.getPaddingTop();
        translateMatrix.setTranslate(dx, dy);

        // Pre-concatenate the current matrix of the canvas with the translation and transformation matrices of the view
        c.concat(translateMatrix);
        c.concat(matrix);
    }
}