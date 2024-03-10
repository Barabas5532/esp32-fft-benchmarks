#include "esp_log.h"
#include "esp_dsp.h"
#define USE_FULL_REPORT
#include "report.inc"
#include "sdkconfig.h"
#include <malloc.h>

#define TAG "app_main"

void app_main(void) {
#ifndef CONFIG_COMPILER_OPTIMIZATION_PERF
  ESP_LOGW(TAG, "WARNING: not optimizing for performance, don't use these "
                "benchmark results");
#endif
  // This test generates benchmark rst table for all available functions
  const size_t test_size = 1024;

  esp_err_t ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG,
             "Not possible to initialize floating point FFT-R2. Error = 0x%x",
             ret);
    abort();
  }
  ret = dsps_fft2r_init_sc16(NULL, CONFIG_DSP_MAX_FFT_SIZE);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Not possible to initialize fixed point FFT-R2. Error = 0x%x",
             ret);
    abort();
  }

  ret = dsps_fft4r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Not possible to initialize fixed point FFT-R4. Error = 0x%x",
             ret);
    abort();
  }

  float *data1 = (float *)memalign(16, test_size * 2 * sizeof(float));
  float *data2 = (float *)memalign(16, test_size * 2 * sizeof(float));
  float *data3 = (float *)memalign(16, test_size * 2 * sizeof(float));

  if (!data1 || !data2 || !data3) {
    ESP_LOGE(TAG, "Failed to allocate buffers");
    abort();
  }

  fir_f32_t fir1;
  dsps_fir_init_f32(&fir1, data1, data2, 256);

  fir_f32_t fir2;
  dsps_fird_init_f32(&fir2, data1, data2, 256, 4);

  float coeffs[5];
  dsps_biquad_gen_lpf_f32(coeffs, 0.1, 1);

#if CONFIG_IDF_TARGET_ESP32
  REPORT_HEADER_ESP32();
#elif CONFIG_IDF_TARGET_ESP32S3
  REPORT_HEADER_ESP32S3();
#endif
  REPORT_SECTION("**FFTs Radix-2 32 bit Floating Point**");

  REPORT_BENCHMARK("dsps_fft2r_fc32 for  64 complex points", dsps_fft2r_fc32,
                   dsps_fft2r_fc32_ansi, data1, 64);

  REPORT_BENCHMARK("dsps_fft2r_fc32 for 128 complex points", dsps_fft2r_fc32,
                   dsps_fft2r_fc32_ansi, data1, 128);

  REPORT_BENCHMARK("dsps_fft2r_fc32 for 256 complex points", dsps_fft2r_fc32,
                   dsps_fft2r_fc32_ansi, data1, 256);

  REPORT_BENCHMARK("dsps_fft2r_fc32 for 512 complex points", dsps_fft2r_fc32,
                   dsps_fft2r_fc32_ansi, data1, 512);

  REPORT_BENCHMARK("dsps_fft2r_fc32 for 1024 complex points", dsps_fft2r_fc32,
                   dsps_fft2r_fc32_ansi, data1, 1024);

  REPORT_SECTION("**FFTs Radix-4 32 bit Floating Point**");

  REPORT_BENCHMARK("dsps_fft4r_fc32 for  64 complex points", dsps_fft4r_fc32,
                   dsps_fft4r_fc32_ansi, data1, 64);

  REPORT_BENCHMARK("dsps_fft4r_fc32 for 256 complex points", dsps_fft4r_fc32,
                   dsps_fft4r_fc32_ansi, data1, 256);

  REPORT_BENCHMARK("dsps_fft4r_fc32 for 1024 complex points", dsps_fft4r_fc32,
                   dsps_fft4r_fc32_ansi, data1, 1024);

  dsps_fft2r_deinit_fc32();
  dsps_fft4r_deinit_fc32();
  dsps_fft2r_deinit_sc16();
  free(data1);
  free(data2);
  free(data3);
}
