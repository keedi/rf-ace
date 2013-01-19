#ifndef RFACE_NEWTEST_HPP
#define RFACE_NEWTEST_HPP

#include <cstdlib>
#include <cmath>
#include "options.hpp"
#include "treedata.hpp"
#include "rf_ace.hpp"
#include "newtest.hpp"

using namespace std;
using datadefs::num_t;

void rface_newtest_RF_train_test_classification();
void rface_newtest_RF_train_test_regression();
void rface_newtest_GBT_train_test_classification();
void rface_newtest_GBT_train_test_regression();

void rface_newtest() {
  
  newtest( "Testing RF for classification", &rface_newtest_RF_train_test_classification );
  newtest( "Testing RF for regression", &rface_newtest_RF_train_test_regression );
  newtest( "Testing GBT for classification", &rface_newtest_GBT_train_test_classification );
  newtest( "Testing GBT for regression", &rface_newtest_GBT_train_test_regression );
  newtest( "Testing save/load RF for classification", &rface_newtest_RF_save_load_classification);
  newtest( "Testing save/load RF for regression", &rface_newtest_RF_save_load_regression);
  newtest( "")
  
}

RFACE::TestOutput make_predictions(ForestOptions& forestOptions, const string& targetStr) {

  string fileName = "test_103by300_mixed_nan_matrix.afm";
  Treedata trainData(fileName,'\t',':',false);
  size_t targetIdx = trainData.getFeatureIdx(targetStr);
  vector<num_t> weights = trainData.getFeatureWeights();
  weights[targetIdx] = 0;

  RFACE rface;

  rface.train(&trainData,targetIdx,weights,&forestOptions);
  
  return( rface.test(&trainData) );
  
}

num_t classification_error(const RFACE::TestOutput& predictions) { 
 
  num_t pError = 0.0;
  num_t n = static_cast<num_t>(predictions.catPredictions.size());
  for ( size_t i = 0; i < predictions.catPredictions.size(); ++i ) {
    pError += (predictions.catPredictions[i] != predictions.catTrueData[i]) / n;
  }
  
  return(pError);

}

num_t regression_error(const RFACE::TestOutput& predictions) {

  num_t RMSE = 0.0;
  num_t n = static_cast<num_t>(predictions.numPredictions.size());
  for ( size_t i = 0; i < predictions.numPredictions.size(); ++i ) {
    num_t e = predictions.numPredictions[i] - predictions.numTrueData[i];
    RMSE += powf(e,2)/n;
  }
  
  return( sqrt(RMSE) );

}

void rface_newtest_RF_train_test_classification() {
  
  RFACE rface;
  ForestOptions forestOptions;
  forestOptions.setRFDefaults();
  forestOptions.mTry = 50;

  num_t pError = classification_error( make_predictions(forestOptions,"C:class") );

  newassert(pError < 0.2);

}

void rface_newtest_RF_train_test_regression() {
  
  RFACE rface;
  ForestOptions forestOptions;
  forestOptions.setRFDefaults();
  forestOptions.mTry = 50;

  num_t RMSE = regression_error( make_predictions(forestOptions,"N:output") );
  
  newassert(RMSE < 1.0);

}

void rface_newtest_GBT_train_test_classification() {

  RFACE rface;
  ForestOptions forestOptions;
  forestOptions.setGBTDefaults();

  num_t pError = classification_error( make_predictions(forestOptions,"C:class") );

  newassert( pError < 0.2 );

}

void rface_newtest_GBT_train_test_regression() { 

  RFACE rface;
  ForestOptions forestOptions;
  forestOptions.setGBTDefaults();

  num_t RMSE = regression_error( make_predictions(forestOptions,"N:output") );

  newassert(RMSE < 1.0);

}

#endif
