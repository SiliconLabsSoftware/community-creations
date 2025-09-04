set(SDK_PATH "C:/Users/Admin/SimplicityStudio/SDKs/simplicity_sdk")
set(COPIED_SDK_PATH "simplicity_sdk_2025.6.1")
set(PKG_PATH "C:/Users/Admin/.silabs/slt/installs")

add_library(slc_ OBJECT
)

target_include_directories(slc_ PUBLIC
   "../autogen"
)

target_compile_definitions(slc_ PUBLIC
)

target_link_libraries(slc_ PUBLIC
    "-Wl,--start-group"
    "gcc"
    "c"
    "m"
    "nosys"
    "-Wl,--end-group"
)
target_compile_options(slc_ PUBLIC
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
    $<$<COMPILE_LANGUAGE:C>:-Os>
    $<$<COMPILE_LANGUAGE:C>:-g>
    $<$<COMPILE_LANGUAGE:C>:-fno-lto>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
    $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
    $<$<COMPILE_LANGUAGE:CXX>:-Os>
    $<$<COMPILE_LANGUAGE:CXX>:-g>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-lto>
)

set(post_build_command )
set_property(TARGET slc_ PROPERTY C_STANDARD 17)
set_property(TARGET slc_ PROPERTY CXX_STANDARD 17)
set_property(TARGET slc_ PROPERTY CXX_EXTENSIONS OFF)

target_link_options(slc_ INTERFACE
    "SHELL:-Xlinker -Map=$<TARGET_FILE_DIR:>/.map"
    -fno-lto
)

# BEGIN_SIMPLICITY_STUDIO_METADATA=eJztWv9P2zgU/1eqaD9sN5IApxtcBUw76KSe6LVqy3anZYpc2xQfThzZDtBN/O/3nG9t2pS1hbbsBEJp/Pxsf97H9ouf7e9Wp9v+s3Ha97vtdt+qW989q9s4/9Bvfmr4k1meVfcsx/Gse2unKNNrX3RPGz0odvT+LuC1GyoVE+GxZ+05u55VoyEWhIVDEFz0P9qHnvX+xJNeeBRJ8S/Fuga/EZV61MPwC1qZ3LMStVrt6FJwQmUtRIHJRrEWQxoW2TMKjuLYVxppOqFjtBinhQ6W2McivGRD8wowY8lAbnTq7lndxSII4pDpkY0lRRoMUm4GTLkDHlMthL4av/koijjDmeaAU38IAG7RyB1oXwns6ysqAxFQTaWPOaOhdjNL3DFgdwaXO7bSTc2cMHvCIKjHyYhkVG3XnmksuQ1TFhyldsYyaSQ35IxeophD79c4GlBekgwEkuRUBBGUGDAO1kA2yCMkdYUYDOH4CrGwIk+R62kpUOQoBq0qB3Id6BAMTwbv9f3d/d+cd86e4+/vvjs8ODw42P99cgASqrBkkbHj5MidTOWWl2xNx3/Of5KCCdVrtjrnzdNm/x+/1784a7b9Vvvs4jyZWl9gSkoaiBtKYBJeIq7ojmcNYsY1Cxt3mMfEMF3/8nUs7olY4rE0ECTmNJnC+dSrt1qJsAbzNlT1THrseZ51pXVUd93b29ucE6DHVcrtpEoOxUJSo1nLmkuKaRmnQkaSdIydtF1HUR1HToxP08HtDDFOFCMSlEqeeEBHwhkLwSzjShR0sIZxljbh/GKebqZXsJjbcuJZYwrAWlPv/c7/hr4+DSIOM/HnIrDw2jO+4dlQmzXRohoRpNHPxe+zYREqiUQIH4NeknwhcRUSSfrBAwqhxFC9sLgSi/B5D0SY1dERSv8BhckLmSuR+XPTOP78PLTUfjZkxzgvv1mWv1YuQjtnrSK487zp8A4klQFe0u44yAO1bH3/8IrQ1Dc3HnSLamvrQ1NeYG0bT+Wq5EFQReWgBP9ZbDxuYSJ8Linm6hAwjpWnV2uQUwzkdCwaEWhRCGwoKYTuNIoi8iuLk8FLaAfpK0imIavSMWGino9YtwA8rnQidFp3B1QtaJbin7OBRHL0scwrxhUkVaourBgsqhgKNVIVyiZcbidRK6RMopl0aCGFyeFwFl7DaDIMiEQ4V8fJR0ULRUn7pswN4jGdP0oWQWA6BKr7AYZcy0m8fUiQJCUAeO9wQ+3f3c1B8Pbt3sFmMNwiGSYLScT5lrqhgEDvtETbBhFRgkLNcAlHsqrYdIdI6lMphVTbgmI0AvYt2ZYqYVDs24YgEDqIhz6nN7Q8Okm+77cgiABdU/P5AlYD46cnJuE0kgdU53kMW2lyvIzbeKCNKFoCUBTNdyIpqKU8yROxlA9hf9qn2J8TyeZJWj+ilfiZdXj250y2RY7WimoZnub6H7u9+BrlCRlaN55luJnnGO3hNphZB5rq5eQctWJF6adpP0BRGcnfWX1ezYZV5/Gr1+2Lfuei7581u2/cV6+zE8u/PrQab5yk8GNZnEHOFJ4XWZnazpnSRfUF7MtQ2FyLqrBpjOdpGd42zomzNkYg1mK/7qfEEg09E6IhJYNkn8mUcIZh7EyMxgFSdNqiiQqntFMlx5jsCHNqycG4JzT+hyUrIrgftxZQpYAFm9NwqK+Od7dEehQtR/uk/gvxjyM+83oL0m60c8ovORqqTTL9me/YNpQ26wtb3t7Zig4DGuqqT+ZGfMUS3JWY4+SFu8W4W7Y6zCSOOQQwNKIhoSEerb4L8HysCgVEYJwNHhO/P8Y7PIEpY0+zZBdtcBe28kR023vzDx0wLrVFHEHZpFd6mkYnQGspvQW6V7BpI2czpWOxtZ6CTKmXrxRWlMjLlTbaZ04YvRWPT3LvX32IMi2eHSjV53rddqfR7TeT+2XfzaW4uge/nmcpdENh7Al8/QlJhgacKiOum4dRMH/gaSIGWuT6XKS3BPOMev6iWGCuEDI98hW59vM7dHn2ffow55DFDb/tIDC3WdPKzEGFMtftvlr3/wFqnk9y=END_SIMPLICITY_STUDIO_METADATA
