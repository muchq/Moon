load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

########################################################################################
##################################################
###################
#
#                             scala stuff
#
##############################################################
########################################################################################
SKYLIB_VERSION = "1.0.3"

http_archive(
    name = "bazel_skylib",
    sha256 = "1c531376ac7e5a180e0237938a2536de0c54d93f5c278634818e0efc952dd56c",
    type = "tar.gz",
    url = "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/{}/bazel-skylib-{}.tar.gz".format(SKYLIB_VERSION, SKYLIB_VERSION),
)

RULES_SCALA_VERSION = "887c9be387734d2a49adab441d7a68414e30cbee"

http_archive(
    name = "io_bazel_rules_scala",
    strip_prefix = "rules_scala-%s" % RULES_SCALA_VERSION,
    type = "zip",
    url = "https://github.com/bazelbuild/rules_scala/archive/%s.zip" % RULES_SCALA_VERSION,
)

load("@io_bazel_rules_scala//:scala_config.bzl", "scala_config")

scala_config(scala_version = "3.1.0")

load("@io_bazel_rules_scala//scala:scala.bzl", "scala_repositories")

scala_repositories()

load("@io_bazel_rules_scala//scala:toolchains.bzl", "scala_register_toolchains")

scala_register_toolchains()

########################################################################################
##################################################
###################
#
#                              java stuff
#
##############################################################
########################################################################################

RULES_JVM_EXTERNAL_TAG = "4.4.2"

RULES_JVM_EXTERNAL_SHA = "735602f50813eb2ea93ca3f5e43b1959bd80b213b836a07a62a29d757670b77b"

http_archive(
    name = "rules_jvm_external",
    sha256 = RULES_JVM_EXTERNAL_SHA,
    strip_prefix = "rules_jvm_external-%s" % RULES_JVM_EXTERNAL_TAG,
    url = "https://github.com/bazelbuild/rules_jvm_external/archive/refs/tags/%s.zip" % RULES_JVM_EXTERNAL_TAG,
)

DAGGER_TAG = "2.44"

DAGGER_SHA = "8c0876d46e8ce9332c4d4fbc2444420e0d75f041b3d4bab8313d2542d1e758ff"

http_archive(
    name = "dagger",
    sha256 = DAGGER_SHA,
    strip_prefix = "dagger-dagger-%s" % DAGGER_TAG,
    urls = ["https://github.com/google/dagger/archive/dagger-%s.zip" % DAGGER_TAG],
)

IO_GRPC_GRPC_JAVA_TAG = "1.50.2"

IO_GRPC_GRPC_JAVA_SHA = "9eca289bcc59511a1e22e556c9b460ba9c05129662395af4431f472b642a6220"

http_archive(
    name = "io_grpc_grpc_java",
    sha256 = IO_GRPC_GRPC_JAVA_SHA,
    strip_prefix = "grpc-java-%s" % IO_GRPC_GRPC_JAVA_TAG,
    url = "https://github.com/grpc/grpc-java/archive/%s.zip" % IO_GRPC_GRPC_JAVA_TAG,
)

load("@rules_jvm_external//:repositories.bzl", "rules_jvm_external_deps")

rules_jvm_external_deps()

load("@rules_jvm_external//:setup.bzl", "rules_jvm_external_setup")

rules_jvm_external_setup()

load("@rules_jvm_external//:defs.bzl", "maven_install")
load("@dagger//:workspace_defs.bzl", "DAGGER_ARTIFACTS", "DAGGER_REPOSITORIES")
load("@io_grpc_grpc_java//:repositories.bzl", "IO_GRPC_GRPC_JAVA_ARTIFACTS")
load("@io_grpc_grpc_java//:repositories.bzl", "IO_GRPC_GRPC_JAVA_OVERRIDE_TARGETS")
load("@io_grpc_grpc_java//:repositories.bzl", "grpc_java_repositories")

grpc_java_repositories()

load("@com_google_protobuf//:protobuf_deps.bzl", "PROTOBUF_MAVEN_ARTIFACTS")
load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

protobuf_deps()

ASSERTJ_VERSION = "3.23.1"

GUAVA_VERSION = "31.1-jre"

JACKSON_VERSION = "2.13.4"

JUNIT4_VERSION = "4.13.2"

LOGBACK_VERSION = "1.4.3"

NETTY_VERSION = "4.1.84.Final"

RESTEASY_VERSION = "6.2.0.Final"

SENTRY_VERSION = "6.4.2"

SLF4J_VERSION = "2.0.3"

maven_install(
    artifacts = DAGGER_ARTIFACTS + IO_GRPC_GRPC_JAVA_ARTIFACTS + PROTOBUF_MAVEN_ARTIFACTS + [
        "org.assertj:assertj-core:%s" % ASSERTJ_VERSION,
        "org.slf4j:slf4j-api:%s" % SLF4J_VERSION,
        "ch.qos.logback:logback-classic:%s" % LOGBACK_VERSION,
        "io.sentry:sentry-logback:%s" % SENTRY_VERSION,
        "junit:junit:%s" % JUNIT4_VERSION,
        "com.google.guava:guava:%s" % GUAVA_VERSION,
        "io.netty:netty-all:%s" % NETTY_VERSION,
        "com.fasterxml.jackson.core:jackson-core:%s" % JACKSON_VERSION,
        "com.fasterxml.jackson.core:jackson-databind:%s" % JACKSON_VERSION,
        "com.fasterxml.jackson.datatype:jackson-datatype-jdk8:%s" % JACKSON_VERSION,
        "com.fasterxml.jackson.datatype:jackson-datatype-guava:%s" % JACKSON_VERSION,
        "com.fasterxml.jackson.datatype:jackson-datatype-jsr310:%s" % JACKSON_VERSION,
        "javax.inject:javax.inject:1",
    ],
    generate_compat_repositories = True,
    override_targets = IO_GRPC_GRPC_JAVA_OVERRIDE_TARGETS,
    repositories = DAGGER_REPOSITORIES + [
        "https://maven.google.com",
        "https://repo1.maven.org/maven2",
        "https://repo.maven.apache.org/maven2/",
    ],
)

load("@maven//:compat.bzl", "compat_repositories")

compat_repositories()

########################################################################################
##################################################
###################
#
#                             proto stuff
#
##############################################################
########################################################################################

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")

rules_proto_dependencies()

rules_proto_toolchains()
