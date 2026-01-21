#ifndef __USER_COORD_H__
#define __USER_COORD_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"

/* 常量定义 --------------------------------------------------------------*/
#define USER_PI        (3.14159265358979323846f)  // π
#define USER_TWO_PI    (6.28318530717958647692f)  // 2π
#define USER_HALF_PI   (1.57079632679489661923f)  // π/2
#define PRECISION      (1e-6f)                    // 浮点数精度阈值

/* 类型定义 --------------------------------------------------------------*/

/**
 * @brief 笛卡尔坐标点
 * @note 右手坐标系
 */
typedef struct {
    float x;  // X轴坐标
    float y;  // Y轴坐标
    float z;  // Z轴坐标
} CartesianCoord_Point;

/**
 * @brief 球坐标点
 * @note 半径 + 偏航角 + 俯仰角
 */
typedef struct {
    float radius;  // 半径
    float yaw;     // 偏航角 (度)
    float pitch;   // 俯仰角 (度)
} PolarCoord_Point;

/**
 * @brief 四元数
 * @note 用于表示三维旋转, 避免万向锁问题
 */
typedef struct {
    float w;  // 实部
    float x;  // 虚部i系数
    float y;  // 虚部j系数
    float z;  // 虚部k系数
} Quaternion;

/**
 * @brief 欧拉角
 * @note ZYX旋转顺序(Yaw-Pitch-Roll), 单位:度
 */
typedef struct {
    float roll;   // 横滚角
    float pitch;  // 俯仰角
    float yaw;    // 偏航角
} EulerAngles;

/**
 * @brief 3x3旋转矩阵
 * @note 用于坐标系旋转变换
 */
typedef struct {
    float m[3][3];  // 矩阵元素[行][列]
} RotationMatrix;

/* 函数声明 ------------------------------------------------------------------*/

/* 基础数学函数 */
float Math_Deg2Rad(float deg);
float Math_Rad2Deg(float rad);
float Math_WrapAngleRad(float rad);
float Math_WrapAngleDeg(float deg);

/* 笛卡尔坐标运算 */
float Distance_Cartesian(const CartesianCoord_Point* p1, const CartesianCoord_Point* p2);
float Magnitude_Cartesian(const CartesianCoord_Point* point);
float AngleDifference_Cartesian(const CartesianCoord_Point* p1, const CartesianCoord_Point* p2);
void  Add_Cartesian(const CartesianCoord_Point* p1, const CartesianCoord_Point* p2, CartesianCoord_Point* result);
void  Subtract_Cartesian(const CartesianCoord_Point* p1, const CartesianCoord_Point* p2, CartesianCoord_Point* result);
void  Scale_Cartesian(const CartesianCoord_Point* point, float scale, CartesianCoord_Point* result);
void  Normalize_Cartesian(const CartesianCoord_Point* point, CartesianCoord_Point* result);
float DotProduct_Cartesian(const CartesianCoord_Point* p1, const CartesianCoord_Point* p2);
void  CrossProduct_Cartesian(const CartesianCoord_Point* p1, const CartesianCoord_Point* p2, CartesianCoord_Point* result);
void  Project_Cartesian(const CartesianCoord_Point* point, const CartesianCoord_Point* onto, CartesianCoord_Point* result);
void  Lerp_Cartesian(const CartesianCoord_Point* p1, const CartesianCoord_Point* p2, float t, CartesianCoord_Point* result);

/* 球坐标运算 */
float Distance_Polar(const PolarCoord_Point* p1, const PolarCoord_Point* p2);
float AbsAngleDifference_Polar(const PolarCoord_Point* p1, const PolarCoord_Point* p2);
float YawDifference_Polar(const PolarCoord_Point* p1, const PolarCoord_Point* p2);
float PitchDifference_Polar(const PolarCoord_Point* p1, const PolarCoord_Point* p2);
void  Add_Polar(const PolarCoord_Point* p1, const PolarCoord_Point* p2, PolarCoord_Point* result);
void  Subtract_Polar(const PolarCoord_Point* p1, const PolarCoord_Point* p2, PolarCoord_Point* result);
void  Scale_Polar(const PolarCoord_Point* point, float scale, PolarCoord_Point* result);
void  Normalize_Polar(const PolarCoord_Point* point, PolarCoord_Point* result);
void  Lerp_Polar(const PolarCoord_Point* p1, const PolarCoord_Point* p2, float t, PolarCoord_Point* result);

/* 坐标系转换 */
void CartesianToPolar(const CartesianCoord_Point* cartesian_point, PolarCoord_Point* result);
void PolarToCartesian(const PolarCoord_Point* polar_point, CartesianCoord_Point* result);

/* 坐标旋转 */
void RotateX_Cartesian(const CartesianCoord_Point* point, float angle_deg, CartesianCoord_Point* result);
void RotateY_Cartesian(const CartesianCoord_Point* point, float angle_deg, CartesianCoord_Point* result);
void RotateZ_Cartesian(const CartesianCoord_Point* point, float angle_deg, CartesianCoord_Point* result);

void RotateByMatrix_Cartesian(const CartesianCoord_Point* point, const RotationMatrix* matrix, CartesianCoord_Point* result);
void RotateByQuaternion_Cartesian(const CartesianCoord_Point* point, const Quaternion* quat, CartesianCoord_Point* result);

/* 四元数运算 */
void  Quaternion_Identity(Quaternion* quat);
void  Quaternion_FromAxisAngle(const CartesianCoord_Point* axis, float angle_deg, Quaternion* quat);
void  Quaternion_FromEuler(const EulerAngles* euler, Quaternion* quat);
void  Quaternion_ToEuler(const Quaternion* quat, EulerAngles* euler);
void  Quaternion_Multiply(const Quaternion* q1, const Quaternion* q2, Quaternion* result);
void  Quaternion_Conjugate(const Quaternion* quat, Quaternion* result);
void  Quaternion_Normalize(const Quaternion* quat, Quaternion* result);
float Quaternion_Magnitude(const Quaternion* quat);
void  Quaternion_Slerp(const Quaternion* q1, const Quaternion* q2, float t, Quaternion* result);

/* 旋转矩阵运算 */
void RotationMatrix_Identity(RotationMatrix* matrix);
void RotationMatrix_FromEuler(const EulerAngles* euler, RotationMatrix* matrix);
void RotationMatrix_FromQuaternion(const Quaternion* quat, RotationMatrix* matrix);
void RotationMatrix_ToQuaternion(const RotationMatrix* matrix, Quaternion* quat);
void RotationMatrix_Multiply(const RotationMatrix* m1, const RotationMatrix* m2, RotationMatrix* result);
void RotationMatrix_Transpose(const RotationMatrix* matrix, RotationMatrix* result);

/* 欧拉角运算 */
void EulerAngles_FromQuaternion(const Quaternion* quat, EulerAngles* euler);
void EulerAngles_FromRotationMatrix(const RotationMatrix* matrix, EulerAngles* euler);
void EulerAngles_Normalize(const EulerAngles* euler, EulerAngles* result);

/* 坐标系变换 */
void TransformPoint_Cartesian(const CartesianCoord_Point* point, const CartesianCoord_Point* origin, const RotationMatrix* rotation, CartesianCoord_Point* result);
void InverseTransformPoint_Cartesian(const CartesianCoord_Point* point, const CartesianCoord_Point* origin, const RotationMatrix* rotation, CartesianCoord_Point* result);

#endif //__USER_COORD_H__

