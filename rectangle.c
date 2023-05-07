
Rect rectXYWH(I32 x, I32 y, I32 w, I32 h) {
    Rect result;
    result.x = x;
    result.y = y;
    result.w = w;
    result.h = h;
    return result;
}

Rect rectWH(I32 w, I32 h) {
    return rectXYWH(0, 0, w, h);
}

// GET

I32 rectLeft(Rect rect) {
    return rect.x;
}

I32 rectTop(Rect rect) {
    return rect.y;
}

I32 rectRight(Rect rect) {
    return rect.x + rect.w;
}

I32 rectBottom(Rect rect) {
    return rect.y + rect.h;
}

I32 rectCenter(Rect rect) {
    return rect.x + rect.w / 2;
}

I32 rectMiddle(Rect rect) {
    return rect.y + rect.h / 2;
}

Point rectLeftTop(Rect rect) {
    Point result;
    result.x = rectLeft(rect);
    result.y = rectTop(rect);
    return result;
}

Point rectLeftMiddle(Rect rect) {
    Point result;
    result.x = rectLeft(rect);
    result.y = rectMiddle(rect);
    return result;
}

Point rectLeftBottom(Rect rect) {
    Point result;
    result.x = rectLeft(rect);
    result.y = rectBottom(rect);
    return result;
}


Point rectCenterTop(Rect rect) {
    Point result;
    result.x = rectCenter(rect);
    result.y = rectTop(rect);
    return result;
}

Point rectCenterMiddle(Rect rect) {
    Point result;
    result.x = rectCenter(rect);
    result.y = rectMiddle(rect);
    return result;
}

Point rectCenterBottom(Rect rect) {
    Point result;
    result.x = rectCenter(rect);
    result.y = rectBottom(rect);
    return result;
}


Point rectRightTop(Rect rect) {
    Point result;
    result.x = rectRight(rect);
    result.y = rectTop(rect);
    return result;
}

Point rectRightMiddle(Rect rect) {
    Point result;
    result.x = rectRight(rect);
    result.y = rectMiddle(rect);
    return result;
}

Point rectRightBottom(Rect rect) {
    Point result;
    result.x = rectRight(rect);
    result.y = rectBottom(rect);
    return result;
}

// SET

Rect rectLeftSet(Rect rect, I32 x) {
    Rect result = rect;
    result.x = x;
    return result;
}

Rect rectTopSet(Rect rect, I32 y) {
    Rect result = rect;
    result.y = y;
    return result;
}

Rect rectRightSet(Rect rect, I32 x) {
    Rect result = rect;
    result.x = x - rect.w;
    return result;
}

Rect rectBottomSet(Rect rect, I32 y) {
    Rect result = rect;
    result.y = y - rect.h;
    return result;
}


Rect rectCenterSet(Rect rect, I32 x) {
    Rect result = rect;
    result.x = x - rect.w / 2;
    return result;
}

Rect rectMiddleSet(Rect rect, I32 y) {
    Rect result = rect;
    result.y = y - rect.h / 2;
    return result;
}

Rect rectLeftTopSet(Rect rect, Point p) {
    Rect result = rect;
    result = rectLeftSet(result, p.x);
    result = rectTopSet(result, p.y);
    return result;
}

Rect rectLeftMiddleSet(Rect rect, Point p) {
    Rect result = rect;
    result = rectLeftSet(result, p.x);
    result = rectMiddleSet(result, p.y);
    return result;
}

Rect rectLeftBottomSet(Rect rect, Point p) {
    Rect result = rect;
    result = rectLeftSet(result, p.x);
    result = rectBottomSet(result, p.y);
    return result;
}


Rect rectCenterTopSet(Rect rect, Point p) {
    Rect result = rect;
    result = rectCenterSet(result, p.x);
    result = rectTopSet(result, p.y);
    return result;
}

Rect rectCenterMiddleSet(Rect rect, Point p) {
    Rect result = rect;
    result = rectCenterSet(result, p.x);
    result = rectMiddleSet(result, p.y);
    return result;
}

Rect rectCenterBottomSet(Rect rect, Point p) {
    Rect result = rect;
    result = rectCenterSet(result, p.x);
    result = rectBottomSet(result, p.y);
    return result;
}

Rect rectRightTopSet(Rect rect, Point p) {
    Rect result = rect;
    result = rectRightSet(result, p.x);
    result = rectTopSet(result, p.y);
    return result;
}

Rect rectRightMiddleSet(Rect rect, Point p) {
    Rect result = rect;
    result = rectRightSet(result, p.x);
    result = rectMiddleSet(result, p.y);
    return result;
}

Rect rectRightBottomSet(Rect rect, Point p) {
    Rect result = rect;
    result = rectRightSet(result, p.x);
    result = rectBottomSet(result, p.y);
    return result;
}





// OTHER

B32 rectContains(Rect rect, I32 x, I32 y) {
    return (x >= rectLeft(rect) && x < rectRight(rect) && y >= rectTop(rect) && y < rectBottom(rect));
}

B32 rectIntersects(Rect a, Rect b) {
    return (rectLeft(a) < rectRight(b) && rectRight(a) > rectLeft(b) &&
            rectTop(a) < rectBottom(b) && rectBottom(a) > rectTop(b)); // NOTE(Edvard): ???
}


Rect rectMove(Rect rect, I32 x, I32 y) {
    Rect result = rect;
    result.x += x;
    result.y += y;
    return result;
}


Rect rectTiled(Rect rect, I32 x, I32 y) {
    Rect result = rect;
    result.x = rect.x + rect.w * x;
    result.y = rect.y + rect.h * y;
    return result;
}


Rect Rect_Shrink(Rect rect, I32 x, I32 y) {
    Rect result = rect;
    result.x += x;
    result.y += y;
    result.w -= x * 2;
    result.h -= y * 2;
    return result;
}

Rect Rect_Grow(Rect rect, I32 x, I32 y) {
    Rect result = rect;
    result.x -= x;
    result.y -= y;
    result.w += x * 2;
    result.h += y * 2;
    return result;
}

Rect Rect_CutLeft(Rect rect, I32 size, I32 margin) {
    Rect result = rect;
    result.w = size;
    result = Rect_Shrink(result, margin, margin);
    return result;
}

Rect Rect_CutTop(Rect rect, I32 size, I32 margin) {
    Rect result = rect;
    result.h = size;
    result = Rect_Shrink(result, margin, margin);
    return result;
}


Rect Rect_CutRight(Rect rect, I32 size, I32 margin) {
    Rect result = rect;
    result.x = (rect.x + rect.w) - size;
    result.w = size;
    result = Rect_Shrink(result, margin, margin);
    return result;
}

Rect Rect_CutBottom(Rect rect, I32 size, I32 margin) {
    Rect result = rect;
    result.y = (rect.y + rect.h) - size;
    result.h = size;
    result = Rect_Shrink(result, margin, margin);
    return result;
}

Rect Rect_DivideX(Rect rect, I32 margin, I32 count, I32 index) {
    Rect result = rect;
    result.w /= count;
    result.x += result.w * index;
    result = Rect_Shrink(result, margin, margin);
    return result;
}

Rect Rect_DivideY(Rect rect, I32 margin, I32 count, I32 index) {
    Rect result = rect;
    result.h /= count;
    result.y += result.h * index;
    result = Rect_Shrink(result, margin, margin);
    return result;
}
