/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of N8.
 * 
 * N8 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * N8 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with N8. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef N8_STDLIB_ML_CC
#define N8_STDLIB_ML_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "N8Library.hpp"

N8_LIB_START

N8_FUNC(ml_trendline_calculate);
N8_FUNC(ml_trendline_calculateRmse);
N8_FUNC(ml_trendline_predict);

N8_FUNC(ml_ann_create);
N8_FUNC(ml_ann_fromMnist);
N8_FUNC(ml_ann_fromModelFile);
N8_FUNC(ml_ann_train);
N8_FUNC(ml_ann_predict);
N8_FUNC(ml_ann_calculateMseLoss);
N8_FUNC(ml_ann_computeOutputGradient);
N8_FUNC(ml_ann_computeAccuracy);
N8_FUNC(ml_ann_isCorrectPrediction);
N8_FUNC(ml_ann_saveModel);

N8_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
