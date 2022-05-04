param([string[]] $tests)

function Invoke-Tests {
    param([string[]] $configs)
    foreach ($config in $configs){
        ./bin/GAME_APPLICATION -f=2 -c="$config"
    }
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "mesh-test")){
    $configs = @(
        # Default Mesh
        "config/mesh-test/default-0.jsonc",
        "config/mesh-test/default-1.jsonc",
        "config/mesh-test/default-2.jsonc",
        "config/mesh-test/default-3.jsonc",

        # Monkey Mesh
        "config/mesh-test/monkey-0.jsonc",
        "config/mesh-test/monkey-1.jsonc",
        "config/mesh-test/monkey-2.jsonc",
        "config/mesh-test/monkey-3.jsonc"
    )
    Write-Output ""
    Write-Output "Running mesh-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "transform-test")){
    $configs = @(
        "config/transform-test/test-0.jsonc"
    )
    Write-Output ""
    Write-Output "Running transform-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "pipeline-test")){
    $configs = @(
        # Face Culling
        "config/pipeline-test/fc-0.jsonc",
        "config/pipeline-test/fc-1.jsonc",
        "config/pipeline-test/fc-2.jsonc",
        "config/pipeline-test/fc-3.jsonc",

        # Depth Testing
        "config/pipeline-test/dt-0.jsonc",
        "config/pipeline-test/dt-1.jsonc",
        "config/pipeline-test/dt-2.jsonc",

        # Blending
        "config/pipeline-test/b-0.jsonc",
        "config/pipeline-test/b-1.jsonc",
        "config/pipeline-test/b-2.jsonc",
        "config/pipeline-test/b-3.jsonc",
        "config/pipeline-test/b-4.jsonc",

        # Mask
        "config/pipeline-test/cm-0.jsonc",
        "config/pipeline-test/dm-0.jsonc"
    )
    Write-Output ""
    Write-Output "Running pipeline-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "texture-test")){
    $configs = @(
        "config/texture-test/test-0.jsonc"
    )
    Write-Output ""
    Write-Output "Running texture-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "sampler-test")){
    $configs = @(
        "config/sampler-test/test-0.jsonc",
        "config/sampler-test/test-1.jsonc",
        "config/sampler-test/test-2.jsonc",
        "config/sampler-test/test-3.jsonc",
        "config/sampler-test/test-4.jsonc",
        "config/sampler-test/test-5.jsonc",
        "config/sampler-test/test-6.jsonc",
        "config/sampler-test/test-7.jsonc"
    )
    Write-Output ""
    Write-Output "Running sampler-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "material-test")){
    $configs = @(
        "config/material-test/test-0.jsonc",
        "config/material-test/test-1.jsonc"
    )
    Write-Output ""
    Write-Output "Running material-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "entity-test")){
    $configs = @(
        "config/entity-test/test-0.jsonc",
        "config/entity-test/test-1.jsonc"
    )
    Write-Output ""
    Write-Output "Running entity-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "renderer-test")){
    $configs = @(
        "config/renderer-test/test-0.jsonc",
        "config/renderer-test/test-1.jsonc"
    )
    Write-Output ""
    Write-Output "Running renderer-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "sky-test")){
    $configs = @(
        "config/sky-test/test-0.jsonc",
        "config/sky-test/test-1.jsonc"
    )
    Write-Output ""
    Write-Output "Running sky-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "postprocess-test")){
    $configs = @(
        "config/postprocess-test/test-0.jsonc",
        "config/postprocess-test/test-1.jsonc",
        "config/postprocess-test/test-2.jsonc",
        "config/postprocess-test/test-3.jsonc"
    )
    Write-Output ""
    Write-Output "Running postprocess-test:"
    Write-Output ""
    Invoke-Tests $configs
}