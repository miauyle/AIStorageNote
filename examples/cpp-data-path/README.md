# C++ data-path learning exercises

Read the [Chinese hands-on tutorial](https://miauyle.github.io/ai-storage-notes/docs/04_CPP_Labs/)
for explanations, expected output, deliberate bug/fix exercises and scope boundaries.

Requirements: C++17 compiler, CMake >= 3.16, libcurl development package >= 7.85,
Python >= 3.9 for the local fixture tests. No GPU or S3 credentials are needed for tests.

From the repository root:

```bash
cmake -S examples/cpp-data-path -B build/cpp-labs -DCMAKE_BUILD_TYPE=Debug
cmake --build build/cpp-labs --parallel 2
ctest --test-dir build/cpp-labs --output-on-failure
```

Use `-DBUILD_S3_PROBE=OFF` for the standard-library-only ownership exercise.
The intentional use-after-free example is opt-in (`BUILD_BAD_EXAMPLE=ON`),
not part of the default build or tests. It must fail under AddressSanitizer.

The HTTPS probe accepts a GET URL through `S3_PROBE_URL`, checks HTTP 206,
Content-Range, byte count, and every byte against a deterministic fixture.
Do not log or commit the URL. Use only a new key in an authorized test bucket.
The program never uploads objects or creates buckets; fixture generation is local
and refuses to overwrite an existing file. TLS verification is enabled.

Local HTTP tests are NOT S3 integration tests. One real Range GET is only a slice
of M0, not the complete benchmark/retry exercise. None of this implements CUDA,
GPU memory, RDMA, KV caching, or the M1 state-machine simulation.
