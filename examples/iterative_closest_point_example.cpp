#include <cilantro/iterative_closest_point.hpp>
#include <cilantro/ply_io.hpp>
#include <cilantro/visualizer.hpp>

int main(int argc, char ** argv) {

    PointCloud dst, src;
    readPointCloudFromPLYFile(argv[1], dst);

    src = dst;
    for (size_t i = 0; i < src.size(); i++) {
        src.points[i] += 0.01f*Eigen::Vector3f::Random();
    }

    Eigen::Matrix3f R_ref(Eigen::Matrix3f::Random());
    Eigen::JacobiSVD<Eigen::Matrix3f> svd(R_ref, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::Matrix3f U(svd.matrixU());
    Eigen::Matrix3f Vt(svd.matrixV().transpose());
    Eigen::Matrix3f tmp(U * Vt);
    if (tmp.determinant() < 0) {
        Eigen::Matrix3f S(Eigen::Matrix3f::Identity());
        S(2, 2) = -1;
        R_ref = U * S * Vt;
    } else {
        R_ref = tmp;
    }
    Eigen::Vector3f t_ref(Eigen::Vector3f::Random());

    src.pointsMatrixMap() = (R_ref*src.pointsMatrixMap()).colwise() + t_ref;
    src.normalsMatrixMap() = R_ref*src.normalsMatrixMap();

    Eigen::Matrix3f R_est;
    Eigen::Vector3f t_est;
    estimateRigidTransformPointToPoint(dst.points, src.points, R_est, t_est);

    std::cout << "TRUE:" << std::endl << R_ref.transpose() << std::endl << t_ref.transpose() << std::endl;
    std::cout << "ESTIMATED:" << std::endl << R_est << std::endl << t_est.transpose() << std::endl;

    src.pointsMatrixMap() = (R_est*src.pointsMatrixMap()).colwise() + t_est;
    src.normalsMatrixMap() = R_est*src.normalsMatrixMap();


    Visualizer viz("win", "disp");

    viz.addPointCloud("dst", dst, Visualizer::RenderingProperties().setDrawingColor(0,0,1));
    viz.addPointCloud("src", src, Visualizer::RenderingProperties().setDrawingColor(1,0,0));


    while (!viz.wasStopped()) {
        viz.spinOnce();
    }

    return 0;
}
