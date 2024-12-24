// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_EXP_FILTER_H_
#define SM_BASE_EXP_FILTER_H_

namespace sm {

// This class can be used, for example, for smoothing the result of bandwidth
// estimation and packet loss estimation.

class ExpFilter {
 public:
  static const float kValueUndefined;

  explicit ExpFilter(float alpha, float max = kValueUndefined)
      : max_(max) {
    Reset(alpha);
  }

  // Resets the filter to its initial state, and resets filter factor base to
  // the given value |alpha|.
  void Reset(float alpha);

  // Applies the filter with a given exponent on the provided sample:
  // y(k) = min(alpha_^ exp * y(k-1) + (1 - alpha_^ exp) * sample, max_).
  float Apply(float exp, float sample);

  // Returns current filtered value.
  float filtered() const { return filtered_; }

  // Changes the filter factor base to the given value |alpha|.
  void UpdateBase(float alpha);

 private:
  float alpha_;  // Filter factor base.
  float filtered_;  // Current filter output.
  const float max_;
};
}  // namespace sm

#endif  // SM_BASE_EXP_FILTER_H_
