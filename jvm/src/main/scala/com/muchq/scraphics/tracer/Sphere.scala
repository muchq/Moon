package com.muchq.scraphics.tracer

case class Sphere(center: Vec3, radius: Double, color: Color, specular: Double, reflective: Double):
  val r2: Double = radius * radius
