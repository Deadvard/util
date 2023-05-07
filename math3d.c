
typedef float F32;
typedef int I32;

typedef struct Vector3D {
    F32 x;
    F32 y;
    F32 z;
} Vector3D;


typedef struct Point3D {
    F32 x;
    F32 y;
    F32 z;
} Point3D;


typedef struct Matrix3D {
    F32 data[3][3];
} Matrix3D;


typedef struct Matrix4D {
    F32 data[4][4];
} Matrix4D;


typedef struct Transform4D {
    F32 data[4][4];
} Transform4D;



static Vector3D Math3D_Multiply3f(F32 *a, F32 b) {
    Vector3D result;

    result.x = a[0] * b;
    result.y = a[1] * b;
    result.z = a[2] * b;

    return result;
}

static F32 Math3D_Dot(F32 *a, F32 *b) {
    F32 result = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    return result;
}

static Vector3D Math3D_Cross(F32 *a, F32 *b) {
    Vector3D result;

    result.x = a[1] * b[2] - a[2] * b[1];
    result.y = a[2] * b[0] - a[0] * b[2];
    result.z = a[0] * b[1] - a[1] * b[0];

    return result;
}


static Vector3D Vector3D_Add(Vector3D a, Vector3D b) {
    Vector3D result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;
}

static Vector3D Vector3D_Subtract(Vector3D a, Vector3D b) {
    Vector3D result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}

static Vector3D Vector3D_Multiply(Vector3D a, F32 b) {
    Vector3D result;

    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;

    return result;
}

static Vector3D Vector3D_Divide(Vector3D a, F32 b) {
    Vector3D result;

    F32 s = 1.0f / b;

    result.x = a.x * s;
    result.y = a.y * s;
    result.z = a.z * s;

    return result;
}

static F32 Vector3D_Dot(Vector3D a, Vector3D b) {
    F32 result = Math3D_Dot(&a.x, &b.x);
    return result;
}

static Vector3D Vector3D_Cross(Vector3D a, Vector3D b) {
    Vector3D result = Math3D_Cross(&a.x, &b.x);
    return result;
}

static F32 Vector3D_Magnitude(Vector3D v) {
    F32 result = Math3D_Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return result;
}

static Vector3D Vector3D_Normalize(Vector3D v) {
    Vector3D result = Vector3D_Divide(v, Vector3D_Magnitude(v));
    return result;
}

static Vector3D Vector3D_Project(Vector3D a, Vector3D b) {
    Vector3D result = Vector3D_Multiply(b, (Vector3D_Dot(a, b) / Vector3D_Dot(b, b)));
    return result;
}

static Vector3D Vector3D_Reject(Vector3D a, Vector3D b) {
    Vector3D result = Vector3D_Subtract(a, Vector3D_Project(a, b));
    return result;
}




static Matrix3D Matrix3D_Create(Vector3D a, Vector3D b, Vector3D c) {
    Matrix3D result;
    result.data[0][0] = a.x;
    result.data[0][1] = a.y;
    result.data[0][2] = a.z;

    result.data[1][0] = b.x;
    result.data[1][1] = b.y;
    result.data[1][2] = b.z;

    result.data[2][0] = c.x;
    result.data[2][1] = c.y;
    result.data[2][2] = c.z;

    return result;
}

static F32 *Matrix3D_Column(Matrix3D *self, I32 column) {
    return self->data[column];
}

static F32 *Matrix3D_At(Matrix3D *self, I32 row, I32 column) {
    F32 *col = Matrix3D_Column(self, column);
    return &col[row];
}

static Matrix3D Matrix3D_Multiply(Matrix3D a, Matrix3D b){
    Matrix3D result;

    *Matrix3D_At(&result, 0, 0) = (*Matrix3D_At(&a, 0, 0) * *Matrix3D_At(&b, 0, 0)) + (*Matrix3D_At(&a, 0, 1) * *Matrix3D_At(&b, 1, 0)) + (*Matrix3D_At(&a, 0, 2) * *Matrix3D_At(&b, 2, 0));
    *Matrix3D_At(&result, 0, 1) = (*Matrix3D_At(&a, 0, 0) * *Matrix3D_At(&b, 0, 1)) + (*Matrix3D_At(&a, 0, 1) * *Matrix3D_At(&b, 1, 1)) + (*Matrix3D_At(&a, 0, 2) * *Matrix3D_At(&b, 2, 1));
    *Matrix3D_At(&result, 0, 2) = (*Matrix3D_At(&a, 0, 0) * *Matrix3D_At(&b, 0, 2)) + (*Matrix3D_At(&a, 0, 1) * *Matrix3D_At(&b, 1, 2)) + (*Matrix3D_At(&a, 0, 2) * *Matrix3D_At(&b, 2, 2));

    *Matrix3D_At(&result, 1, 0) = (*Matrix3D_At(&a, 1, 0) * *Matrix3D_At(&b, 0, 0)) + (*Matrix3D_At(&a, 1, 1) * *Matrix3D_At(&b, 1, 0)) + (*Matrix3D_At(&a, 1, 2) * *Matrix3D_At(&b, 2, 0));
    *Matrix3D_At(&result, 1, 1) = (*Matrix3D_At(&a, 1, 0) * *Matrix3D_At(&b, 0, 1)) + (*Matrix3D_At(&a, 1, 1) * *Matrix3D_At(&b, 1, 1)) + (*Matrix3D_At(&a, 1, 2) * *Matrix3D_At(&b, 2, 1));
    *Matrix3D_At(&result, 1, 2) = (*Matrix3D_At(&a, 1, 0) * *Matrix3D_At(&b, 0, 2)) + (*Matrix3D_At(&a, 1, 1) * *Matrix3D_At(&b, 1, 2)) + (*Matrix3D_At(&a, 1, 2) * *Matrix3D_At(&b, 2, 2));

    *Matrix3D_At(&result, 2, 0) = (*Matrix3D_At(&a, 2, 0) * *Matrix3D_At(&b, 0, 0)) + (*Matrix3D_At(&a, 2, 1) * *Matrix3D_At(&b, 1, 0)) + (*Matrix3D_At(&a, 2, 2) * *Matrix3D_At(&b, 2, 0));
    *Matrix3D_At(&result, 2, 1) = (*Matrix3D_At(&a, 2, 0) * *Matrix3D_At(&b, 0, 1)) + (*Matrix3D_At(&a, 2, 1) * *Matrix3D_At(&b, 1, 1)) + (*Matrix3D_At(&a, 2, 2) * *Matrix3D_At(&b, 2, 1));
    *Matrix3D_At(&result, 2, 2) = (*Matrix3D_At(&a, 2, 0) * *Matrix3D_At(&b, 0, 2)) + (*Matrix3D_At(&a, 2, 1) * *Matrix3D_At(&b, 1, 2)) + (*Matrix3D_At(&a, 2, 2) * *Matrix3D_At(&b, 2, 2));

    return result;
}


static Vector3D Matrix3D_MultiplyVector3D(Matrix3D m, Vector3D v) {
    Vector3D result;

    result.x = *Matrix3D_At(&m, 0, 0) * v.x + *Matrix3D_At(&m, 0, 1) * v.y + *Matrix3D_At(&m, 0, 1) * v.z;
    result.y = *Matrix3D_At(&m, 1, 0) * v.x + *Matrix3D_At(&m, 1, 1) * v.y + *Matrix3D_At(&m, 1, 1) * v.z;
    result.z = *Matrix3D_At(&m, 2, 0) * v.x + *Matrix3D_At(&m, 2, 1) * v.y + *Matrix3D_At(&m, 2, 1) * v.z;

    return result;
}

static Matrix3D Matrix3D_Inverse(Matrix3D m) {
    F32 *a = Matrix3D_Column(&m, 0);
    F32 *b = Matrix3D_Column(&m, 1);
    F32 *c = Matrix3D_Column(&m, 2);


    Vector3D r0 = Math3D_Cross(b, c);
    Vector3D r1 = Math3D_Cross(c, a);
    Vector3D r2 = Math3D_Cross(a, b);

    F32 invDet = 1.0f / Math3D_Dot((F32 *)&r2, c);

    Matrix3D result;

    *Matrix3D_At(&result, 0, 0) = r0.x * invDet;
    *Matrix3D_At(&result, 0, 1) = r0.y * invDet;
    *Matrix3D_At(&result, 0, 2) = r0.z * invDet;

    *Matrix3D_At(&result, 1, 0) = r1.x * invDet;
    *Matrix3D_At(&result, 1, 1) = r1.y * invDet;
    *Matrix3D_At(&result, 1, 2) = r1.z * invDet;

    *Matrix3D_At(&result, 2, 0) = r2.x * invDet;
    *Matrix3D_At(&result, 2, 1) = r2.y * invDet;
    *Matrix3D_At(&result, 2, 2) = r2.z * invDet;

    return result;
}



static Matrix4D Matrix4D_Inverse(Matrix4D m) {
    F32 *a = Matrix4D_Column(&m, 0);
    F32 *b = Matrix4D_Column(&m, 1);
    F32 *c = Matrix4D_Column(&m, 2);
    F32 *d = Matrix4D_Column(&m, 3);


    F32 x = Matrix4D_At(&m, 3, 0);
    F32 y = Matrix4D_At(&m, 3, 1);
    F32 z = Matrix4D_At(&m, 3, 2);
    F32 w = Matrix4D_At(&m, 3, 3);

    Vector3D s = Math3D_Cross(a, b);
    Vector3D t = Math3D_Cross(c, d);
    Vector3D u = Vector3D_Subtract(Math3D_Multiply3f(a, y), Math3D_Multiply3f(b, x));
    Vector3D v = Vector3D_Subtract(Math3D_Multiply3f(c, w), Math3D_Multiply3f(d, z));
    
    F32 invDet = 1.0f / (Vector3D_Dot(s, v) / Vector3D_Dot(t, u));
    s = Vector3D_Multiply(s, invDet);
    t = Vector3D_Multiply(t, invDet);
    u = Vector3D_Multiply(u, invDet);
    v = Vector3D_Multiply(v, invDet);

    Vector3D r0 =      Vector3D_Add(Math3D_Cross(b, (F32 *)&v), Vector3D_Multiply(t, y));
    Vector3D r1 = Vector3D_Subtract(Math3D_Cross((F32 *)&v, a), Vector3D_Multiply(t, x));
    Vector3D r2 =      Vector3D_Add(Math3D_Cross(d, (F32 *)&u), Vector3D_Multiply(s, w));
    Vector3D r3 = Vector3D_Subtract(Math3D_Cross((F32 *)&u, c), Vector3D_Multiply(s, z));

    Matrix4D result;
    
    *Matrix3D_At(&result, 0, 0) = r0.x;
    *Matrix3D_At(&result, 0, 1) = r0.y;
    *Matrix3D_At(&result, 0, 2) = r0.z;
    *Matrix3D_At(&result, 0, 3) = -Math3D_Dot(b, (F32*)&t);
    
    *Matrix3D_At(&result, 1, 0) = r1.x;
    *Matrix3D_At(&result, 1, 1) = r1.y;
    *Matrix3D_At(&result, 1, 2) = r1.z;
    *Matrix3D_At(&result, 1, 3) = Math3D_Dot(a, (F32*)&t);
                                  
    *Matrix3D_At(&result, 2, 0) = r2.x;
    *Matrix3D_At(&result, 2, 1) = r2.y;
    *Matrix3D_At(&result, 2, 2) = r2.z;
    *Matrix3D_At(&result, 2, 3) = -Math3D_Dot(d, (F32*)&s);
    
    *Matrix3D_At(&result, 3, 0) = r3.x;
    *Matrix3D_At(&result, 3, 1) = r3.y;
    *Matrix3D_At(&result, 3, 2) = r3.z;
    *Matrix3D_At(&result, 3, 3) = Math3D_Dot(c, (F32*)&s);
    
    return result;
}













