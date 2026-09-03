plugins {
    alias(libs.plugins.kotlin.jvm)
}

kotlin {
    jvmToolchain(11)
}

sourceSets {
    main {
        kotlin.srcDir("commons")
    }
}

dependencies {
    testImplementation(libs.junit)
}