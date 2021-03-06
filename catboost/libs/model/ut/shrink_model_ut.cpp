#include "model_test_helpers.h"

#include <catboost/libs/algo/apply.h>
#include <catboost/libs/train_lib/train_model.h>

#include <library/unittest/registar.h>

using namespace std;

Y_UNIT_TEST_SUITE(TShrinkModel) {
    Y_UNIT_TEST(TestTruncateModel) {
        NJson::TJsonValue params;
        params.InsertValue("learning_rate", 0.3);
        params.InsertValue("iterations", 10);
        TFullModel model, model2;
        TEvalResult evalResult;

        TPool pool = GetAdultPool();

        TrainModel(params, Nothing(), Nothing(), TClearablePoolPtrs(pool, {&pool}), "", &model, {&evalResult});
        params.InsertValue("iterations", 7);
        TrainModel(params, Nothing(), Nothing(), TClearablePoolPtrs(pool, {&pool}), "", &model2, {&evalResult});

        model.Truncate(0, 7);
        auto result = ApplyModel(model, pool);
        auto result2 = ApplyModel(model, pool);
        UNIT_ASSERT_EQUAL(result.ysize(), result2.ysize());
        for (int idx = 0; idx < result.ysize(); ++idx) {
            UNIT_ASSERT_DOUBLES_EQUAL(result[idx], result2[idx], 1e-6);
        }
        model.Truncate(1, 4);
    }
}
