tux_course_light 0 -on -position { 1 1 0 0 } -diffuse { 1.0 0.9 1.0 1 } \
     -specular { 0.0 0.0 0.0 1 } -ambient { 0.45 0.53 0.75 1.0 }

tux_course_light 1 -on -position { 1 1 2 0 } -specular { 0.8 0.8 0.8 1 }

tux_fog -on -mode linear -density 0.005 -color { 0.9 0.85 0.8 1 } -start 35 \
    -end [tux_get_param forward_clip_distance]

tux_load_texture envmap envmap.png 0
tux_bind_texture terrain_envmap envmap

tux_particle_color { 0.85 0.9 1.0 1.0 }
    
tux_load_texture envmap envmap.png 0
tux_bind_texture terrain_envmap envmap


tux_load_texture alpine1-front cloudyfront.png 0
tux_load_texture alpine1-right cloudyright.png 0
tux_load_texture alpine1-left cloudyleft.png 0
tux_load_texture alpine1-back cloudyback.png 0
tux_load_texture alpine1-top cloudytop.png 0
tux_load_texture alpine1-bottom cloudybottom.png 0

tux_bind_texture sky_front alpine1-front
tux_bind_texture sky_right alpine1-right
tux_bind_texture sky_left alpine1-left
tux_bind_texture sky_back alpine1-back
tux_bind_texture sky_top alpine1-top
tux_bind_texture sky_bottom alpine1-bottom
