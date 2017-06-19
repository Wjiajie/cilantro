#pragma once

#include <cilantro/point_cloud.hpp>

bool estimateRigidTransformPointToPoint(const Eigen::Matrix<float,3,Eigen::Dynamic> &dst,
                                        const Eigen::Matrix<float,3,Eigen::Dynamic> &src,
                                        Eigen::Matrix3f &rot_mat,
                                        Eigen::Vector3f &t_vec);

inline bool estimateRigidTransformPointToPoint(const std::vector<Eigen::Vector3f> &dst,
                                               const std::vector<Eigen::Vector3f> &src,
                                               Eigen::Matrix3f &rot_mat,
                                               Eigen::Vector3f &t_vec)
{
    return estimateRigidTransformPointToPoint(Eigen::Map<Eigen::Matrix<float,3,Eigen::Dynamic> >((float *)dst.data(),3,dst.size()), Eigen::Map<Eigen::Matrix<float,3,Eigen::Dynamic> >((float *)src.data(),3,src.size()), rot_mat, t_vec);
}

