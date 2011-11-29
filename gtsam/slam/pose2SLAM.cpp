/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 *  @file  pose2SLAM.cpp
 *  @brief: bearing/range measurements in 2D plane
 *  @author Frank Dellaert
 **/

#include <gtsam/slam/pose2SLAM.h>
#include <gtsam/nonlinear/NonlinearFactorGraph-inl.h>
#include <gtsam/nonlinear/NonlinearOptimizer-inl.h>

// Use pose2SLAM namespace for specific SLAM instance
namespace gtsam {

	INSTANTIATE_NONLINEAR_FACTOR_GRAPH(pose2SLAM::Values)
	INSTANTIATE_NONLINEAR_OPTIMIZER(pose2SLAM::Graph, pose2SLAM::Values)
	template class NonlinearOptimizer<pose2SLAM::Graph, pose2SLAM::Values, GaussianFactorGraph, GaussianSequentialSolver>;

	namespace pose2SLAM {

		/* ************************************************************************* */
		Values circle(size_t n, double R) {
			Values x;
			double theta = 0, dtheta = 2 * M_PI / n;
			for (size_t i = 0; i < n; i++, theta += dtheta)
				x.insert(i, Pose2(cos(theta), sin(theta), M_PI_2 + theta));
			return x;
		}

		/* ************************************************************************* */
		void Graph::addPrior(const Key& i, const Pose2& p,
				const SharedNoiseModel& model) {
			sharedFactor factor(new Prior(i, p, model));
			push_back(factor);
		}

		void Graph::addConstraint(const Key& i, const Key& j, const Pose2& z,
				const SharedNoiseModel& model) {
			sharedFactor factor(new Constraint(i, j, z, model));
			push_back(factor);
		}

		void Graph::addHardConstraint(const Key& i, const Pose2& p) {
			sharedFactor factor(new HardConstraint(i, p));
			push_back(factor);
		}

	/* ************************************************************************* */

	} // pose2SLAM

} // gtsam
