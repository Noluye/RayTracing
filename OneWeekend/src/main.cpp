#include <iostream>
#include "Log.h"
#include "color.h"
#include "ray.h"
#include "vec3.h"

bool hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = center - r.origin();
	auto a = dot(r.direction(), r.direction());
	auto b = -2.0 * dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c;
	return (discriminant >= 0);
}

color ray_color(const ray& r) {
	if (hit_sphere(point3(0, 0, -1), 0.5, r))
		return color(1, 0, 0);

	vec3 unit_direction = unit_vector(r.direction());
	// This function will linearly blend white and blue depending on the height of the y
	// coordinate after scaling the ray direction to unit length(so −1.0 < y < 1.0).
	double a = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main()
{
	double aspect_ratio = 16.0 / 9.0;
	int image_width = 400;
	int image_height = std::max(1, int(image_width / aspect_ratio));
	// ---------------------------------------------------------------------------------------
	// Camera
	double focal_length = 1.0;
	// Viewport widths less than one are ok since they are real valued.
	double viewport_height = 2.0;
	// In order for our viewport proportions to exactly match our image proportions, 
	// we use the calculated image aspect ratio to determine our final viewport width.
	double viewport_width = viewport_height * (double(image_width) / image_height);
	point3 camera_center = point3(0, 0, 0);
	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	vec3 viewport_u = vec3(viewport_width, 0, 0);
	vec3 viewport_v = vec3(0, -viewport_height, 0);
	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	vec3 pixel_delta_u = viewport_u / image_width;
	vec3 pixel_delta_v = viewport_v / image_height;
	// Calculate the location of the upper left pixel.(camera space)
	vec3 viewport_upper_left = camera_center
		- vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
	vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	// ---------------------------------------------------------------------------------------
	// Next we will define the camera center: a point in 3D space 
	// from which all scene rays will originate (also commonly referred to as the eye point).
	// The vector from the camera center to the viewport center will be orthogonal to the viewport.
	// We'll initially set the distance between the viewport and the camera center point to be one unit. 
	// This distance is often referred to as the focal length.
	// For simplicity we'll start with the camera center at (0,0,0).
	// We'll also have the y-axis go up, the x-axis to the right,
	// and the negative z-axis pointing in the viewing direction.
	// (This is commonly referred to as right-handed coordinates.)
	// ---------------------------------------------------------------------------------------
	FILE* fp = fopen("image.ppm", "w");
	fprintf(fp, "P3\n%d %d\n255\n", image_width, image_height);
	for (int j = 0; j < image_height; j++) {
		std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) {
			vec3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			// I didn't make ray_direction a unit vector, 
			// because I think not doing that makes for simpler and slightly faster code.
			vec3 ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);
			color pixel_color = ray_color(r);
			// adds content to the file
			write_color(fp, pixel_color);
		}
	}
	fclose(fp);
	std::clog << "\rDone.                 \n";
	// ---------------------------------------------------------------------------------------
}