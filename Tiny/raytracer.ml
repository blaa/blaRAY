 (* Tiny Objective Caml Raytracer	*)
(* (C) 2008 by Tomasz bla Fortuna	*)
(* License: GPLv3+			*)

open Printf

let fabs a = if a < 0.0 then -. a else a

(** Our main hero **)
module Vect =
  struct
    type t = {
      x : float;
      y : float;
      z : float;
    }

    let zero = {
      x = 0.0;
      y = 0.0;
      z = 0.0;
    }

    let print v =
      printf "%5.7f %5.7f %5.7f" v.x v.y v.z

    let create x y z = {
      x = x;
      y = y;
      z = z;
    }

    let get v =
      (v.x, v.y, v.z)

    let add a b = {
      x = a.x +. b.x;
      y = a.y +. b.y;
      z = a.z +. b.z;
    }

    let sub a b = {
      x = a.x -. b.x;
      y = a.y -. b.y;
      z = a.z -. b.z;
    }

    let dot a b =
      (a.x *. b.x) +. (a.y *. b.y) +. (a.z *. b.z)

    let cross a b = {
      x = a.y *. b.z -. a.z *. b.y;
      y = a.z *. b.x -. a.x *. b.z;
      z = a.x *. b.y -. a.y *. b.x;
    }

    let mul f v = {
      x = v.x *. f;
      y = v.y *. f;
      z = v.z *. f;
    }

    let length2 a =
      a.x *. a.x +. a.y *. a.y +. a.z *. a.z

    let length a =
      sqrt (length2 a)

    let normalize a =
      let len = length a in {
	  x = a.x /. len;
	  y = a.y /. len;
	  z = a.z /. len;
        }
  end

(** Module creating/operating on rays **)
module Ray =
  struct
    type t = {
      start : Vect.t; (* Start vector     *)
      dir : Vect.t;   (* Direction vector *)
    }

    let print r =
      printf "Ray: start: (";
      Vect.print r.start;
      printf "), dir: (";
      Vect.print r.dir;
      printf ")\n%!"

    let get_start r =
      r.start

    let get_dir r =
      r.dir

    let point_of_t ~ray ~t =
      Vect.add ray.start (Vect.mul t ray.dir)

    let create ~start ~dir () = {
      start = start;
      dir = dir;
     }

    let gen_reflected_ray ~ray ~normal ~collision_point =
      let a = 2.0 *. (Vect.dot normal ray.dir) in
      let dir = Vect.sub ray.dir (Vect.mul a normal) in
        create ~start:collision_point ~dir:(Vect.normalize dir) ()

    let gen_refracted_ray ~ray ~normal ~collision_point ~cur_n ~new_n =
      let c1 = -. Vect.dot normal ray.dir in
      let n = cur_n /. new_n in
      let c2 = sqrt (1.0 -. n*.n *. (1.0 -. c1 *. c1)) in
      let c3 = n *. c1 -. c2 in
      let dir =
        Vect.add
	  (Vect.mul n ray.dir)
	  (Vect.mul c3 normal)
      in
        create ~start:collision_point ~dir ()

    let ray_of_points ~source ~destination () =
      let dir = Vect.sub destination source in
        create ~start:source ~dir:(Vect.normalize dir) ()

  end

(** Color **)
module Color =
  struct
    type t = {
      r : float;
      g : float;
      b : float;
    }

    let crop a =
      if a > 1.0 then 1.0 else if a < 0.0 then 0.0 else a

    let create r g b = {
      r = crop r;
      g = crop g;
      b = crop b;
    }
    let print c =
      printf "r=%5.2f g=%5.2f b=%5.2f\n%!" c.r c.g c.b

    let red = create 1.0 0.0 0.0
    let green = create 0.0 1.0 0.0
    let blue = create 0.0 0.0 1.0
    let white = create 1.0 1.0 1.0
    let black = create 0.0 0.0 0.0
    let grey = create 0.5 0.5 0.5
    let yellow = create 1.0 1.0 0.0

    let get_r c = c.r
    let get_g c = c.b
    let get_b c = c.g

    let int_r c = int_of_float (c.r *. 255.0)
    let int_g c = int_of_float (c.g *. 255.0)
    let int_b c = int_of_float (c.b *. 255.0)

    let graph_of_color c =
      Graphics.rgb (int_r c) (int_g c) (int_b c)

    let combine a b = {
      r = a.r *. b.r;
      g = a.g *. b.g;
      b = a.b *. b.b;
    }

    let add lst =
      let add a b = {
        r= a.r +. b.r;
        g= a.g +. b.g;
        b= a.b +. b.b;
      } in
      let summed = List.fold_left add black lst in
        {
	  r = crop summed.r;
	  g = crop summed.g;
	  b = crop summed.b;
        }

    let mul a c = {
      r = a *. c.r;
      g = a *. c.g;
      b = a *. c.b;
    }

    let pow c a = {
      r = c.r ** a;
      g = c.g ** a;
      b = c.b ** a;
    }

    let intensity c =
      (c.r +. c.g +. c.b) /. 3.0

    let equal a b =
      if a.r = b.r && a.g = b.g && a.b = b.b then
        true
      else
        false

    let is_zero c =
      if c.r = 0.0 && c.g = 0.0 && c.b = 0.0 then
        true
      else
        false

    let average lst =
      let length = float_of_int (List.length lst) in
      let add a b = {
        r= a.r +. b.r;
        g= a.g +. b.g;
        b= a.b +. b.b;
      } in
      let summed = List.fold_left add black lst in
        {
	  r = summed.r /. length;
	  g = summed.g /. length;
	  b = summed.b /. length;
        }
  end

(* Texture is a function returning color given a x,y coordinates *)
module Texture =
  struct
    type t = float -> float -> Color.t
    let create ~pattern ?(sizex=1.0) ?(sizey=1.0) () =
      match pattern with
        |`PLAIN c ->
	    (* Plain color, returns it's parameter *)
	    (fun x y -> c)
        |`NOISE ->
	    (* Randomly generated color *)
	    (fun x y ->
	       let r = Random.float 1.0
	       and g = Random.float 1.0
	       and b = Random.float 1.0 in
		 Color.create r g b)
        |`XGRADIENT (a, b) ->
	    (* Gradient along X parameter *)
	    (fun x y ->
	       let x = mod_float x sizex in
	       let a_coeff = (sizex -. x) /. sizex in
	       let b_coeff = 1.0 -. a_coeff in
		 Color.add [Color.mul a_coeff a; Color.mul b_coeff b]
	    )
        |`CHECKED (a, c) ->
	    (fun x y ->
	       let halfx = sizex /. 2.0
	       and halfy = sizey /. 2.0 in

	       let x = if x<0.0 then x-.halfx else x
	       and y = if y<0.0 then y-.halfy else y in

	       let x = abs_float (mod_float x sizex)
	       and y = abs_float (mod_float y sizey) in
		 if (x < halfx && y < halfy) || (x > halfx && y > halfy) then
		   a
		 else
		   c
	    )

    let color c = create ~pattern:(`PLAIN c) ()
    let create_plain r g b = color (Color.create r g b)
    let black = color Color.black
    let white = color Color.white

    let red = color Color.red
    let green = color Color.green
    let blue = color Color.blue
  end

module Material =
  struct
    type t = {
      transparency : float;
      reflective : float;
      n : float;
      diffuse : Texture.t;  (* Color filter for... general color. ;p *)
      mirror : Texture.t;   (* Color filter for reflected rays *)
      specular : Texture.t; (* Color filter for light rays *)
      shininess : float
    }

    let create
        ?(shininess=12.0)
        ?(diffuse=Texture.black)
        ?(specular=Texture.create_plain 0.7 0.7 0.7)
        ?(mirror=Texture.create_plain 0.2 0.2 0.2)
        ?(n=1.0)
        ?(transparency=0.0)
        ?(reflective=0.0) () =
      {
        diffuse = diffuse;
        specular = specular;
        mirror = mirror;

        transparency = transparency;
        reflective = reflective;
        n= n;
        shininess = shininess;
      }

    let get_property ~material ~property =
      match property with
        |`N -> material.n
        |`TRANSPARENCY -> material.transparency
        |`REFLECTIVE -> material.reflective
        |`SHININESS -> material.shininess

    let get_texture ~material ~texture =
      match texture with
        |`DIFFUSE -> material.diffuse
        |`SPECULAR -> material.specular
        |`MIRROR -> material.mirror

    let get_color ~material ~texture ~x ~y =
      (get_texture ~material ~texture) x y

  end

(** Camera type **)
module Camera =
  struct
    type t = {
      loc : Vect.t; (* Eyepoint location *)
      dir : Vect.t; (* Viewing direction *)
      top : Vect.t; (* Normalized vector pointing to the 'camera top' *)
      fov : float; (* Camera horizontal field of view *)
      ratio : float; (* Aspect ratio *)
    }
    let pi = 4.0 *. atan(1.0)

    let print c =
      printf "Camera:\n";
      printf "\tloc: ("; Vect.print c.loc; printf ")\n";
      printf "\tdir: ("; Vect.print c.dir; printf ")\n";
      printf "\ttop: ("; Vect.print c.top; printf ")\n";
      printf "\tfov: %5.5f (%5.5f deg)\n" c.fov (c.fov /. pi *. 360.0);
      printf "\tratio: %5.2f\n%!" c.ratio


    let fov_of_degree d =
      d /. 360.0 *. 2.0  *. pi

    let get_loc c = c.loc
    let get_dir c = c.dir
    let get_top c = c.top
    let get_fov c = c.fov
    let get_ratio c = c.ratio

    let create
        ?(loc=Vect.create 0. 0. (-. 1.))
        ?(dir=Vect.create 0. 0. 1.)
        ?(top=Vect.create 0. 1. 0.)
        ?(fov=fov_of_degree 45.0)
        ?(ratio=4.0/.3.0)
        ?(auto_top=false)
        () =
      let dir' = Vect.normalize dir in
      let top' =
        if auto_top then
	  (* top.x = dir.x, top.z = dir.z
	     top.y calculated from constraint: top dot dir = 0 *)
	  let (dx, dy, dz) = Vect.get dir in
	  let ty = -. (dz ** 2.0 +. dx ** 2.0) /. dy in
	    Vect.create dx ty dz
        else
	  top
      in {
	  loc = loc;
	  dir = dir';
	  top = Vect.normalize top';
	  fov = fov;
	  ratio = ratio;
        }

    (* Generates camera-function which generates rays for specified x,y *)
    let gen_ray_of_xy ~cnt ~xres ~yres ~camera =
      let cam_left = Vect.normalize (Vect.cross camera.top camera.dir) in
      let xwidth = tan (camera.fov /. 2.0) in (* Screen size *)
      let ywidth = xwidth /. camera.ratio in
      let x_dist = xwidth /. float_of_int xres in (* Pixel distance on screen *)
      let y_dist = ywidth /. float_of_int yres in
      let x_vect = Vect.mul x_dist cam_left
      and y_vect = Vect.mul y_dist camera.top
      in
      let ray_of_xy x y =
        let x_times = float_of_int (x - (xres / 2))
        and y_times = float_of_int (y - (yres / 2)) in
        let base = Vect.add (Vect.mul x_times x_vect) (Vect.mul y_times y_vect)
        in
	  incr cnt;
	  Ray.create ~dir:(Vect.add base camera.dir) ~start:camera.loc ();
      in
        ray_of_xy
  end

module Scene =
  struct
    let pi = 4.0 *. atan(1.0)
    type plane = {
      normal : Vect.t;
      distance : float;  (* Distance from (0,0) along normal *)
      p_material : Material.t;
    }

    type sphere = {
      center : Vect.t;
      radius : float;
      s_material : Material.t;
    }

    type light = {
      light_pos : Vect.t;
      light_color : Color.t;
    }

    type obj =
      | Plane of plane
      | Sphere of sphere
      | NoObject

    let objects = ref []
    let lights = ref []

    (* Getters *)
    let get_object_material obj =
      match obj with
        |Sphere s -> s.s_material
        |Plane p -> p.p_material
        |NoObject -> failwith "Wrong object specified"

    let get_object_coordinates obj v =
      (* Get object x, y coordinates *)
      match obj with
        |Plane p ->
	    let (x, _, z) = Vect.get v in
	    (x, z) (* FIXME *)
        |Sphere s ->
	    let w = Vect.sub v s.center in
	    let (x,y,z) = Vect.get w
	    and r = s.radius in
	    let v = acos (y /. r) /. pi in
	    let helper = acos (x /. (r *. sin (pi *. v) )) in
	    let u =
	      if y > 0.0 then
                helper /. 2. /. pi
	      else
                (pi +. helper) /. 2. /. pi
	    in
	      (v, u)
        |NoObject -> failwith "Wrong object specified"

    let get_object_color ~obj ~texture ~point =
      let (x, y) = get_object_coordinates obj point in
      let material = get_object_material obj in
      let c = Material.get_color ~material ~texture ~x ~y
      in
        c

    let get_light_position l =
      l.light_pos

    let get_light_color l =
      l.light_color

    let get_normal obj point =
      match obj with
        |Sphere s ->
	    Vect.normalize (Vect.sub point s.center)
        |Plane p ->
	    p.normal
        |_ -> failwith "Wrong object specified"

    (* Adders *)
    let add_sphere ~location ~radius ~material =
      let sphere = {
        center = location;
        radius = radius;
        s_material = material;
      } in
        objects := (Sphere sphere) :: (!objects)

    let add_plane ~distance ~normal ~material =
      let plane = {
        normal = normal;
        distance = distance;
        p_material = material;
      } in
        objects := (Plane plane) :: (!objects)

    let add_light ~location ~color =
      let light = {
        light_pos = location;
        light_color = color;
      } in
        lights := light :: (!lights)

    let add_area_light ~lights ~radius ~location ~color =
      for i = 0 to lights do
        let ra = radius *. 2.0 in
        let rb = radius in

        let v =
	  Vect.create (rb -. Random.float ra)
	    (rb -. Random.float ra)
	    (rb -. Random.float ra)
        in
	  add_light ~location:(Vect.add location v) ~color
      done

    let lights_fold f init =
      List.fold_left f init !lights

    let rec find_collision ~ray =
      let ray_start = Ray.get_start ray
      and ray_dir = Ray.get_dir ray
      and (@) = Vect.dot in

      let collide collision obj =
        let best_t, best_obj, opacity = collision in
        match obj with
	  | Plane p ->
	      (* N[A, B, C] - plane normals
	       * P[x, y, z] point of collision
	       * Rs - Ray start vector; Rd - Ray direction vector
	       *
	       * N . P + D = 0  ;   Rs + t * Rd = P
	       * N . (Rs + t*Rd) + D = 0
	       * N.Rs + t*(N . Rd) + D = 0
	       *
	       *   N . Rs + D
	       * - ---------- = t
	       *     N . Rd
	       *
	       *)
	      let t =
                (-. (p.normal @ ray_start) +. p.distance) /.
		  (p.normal @ ray_dir)
	      in
                if t > 0.0000001 && t < best_t then (
		  let opacity' = 1.0 -.
		    (Material.get_property
		       ~material:(get_object_material obj)
		       ~property:`TRANSPARENCY) in
		  (t, obj, opacity' +. opacity)
                )else (
		  (best_t, best_obj, opacity)
                )
	  | Sphere s ->
	      (* R - radius
	       * C[a,b,c] - sphere center
	       * P[x,y,z] - sphere point
	       *
	       * |P - C| = R
	       * Rs + t*Rd = P
	       * | Rs+t*Rd - C | = R
	       *)
	      let minimal_distance = 0.00001 in
	      let v = Vect.sub ray_start s.center in
	      let v' = Vect.length2 v
	      and d' = Vect.length2 ray_dir
	      and r' = s.radius *. s.radius in
	      let denom = 2.0 *. d'
	      and a = 2.0 *. (v @ ray_dir) in
	      let delta = (a *. a) -. (2.0 *. denom) *. (v' -. r') in
                if delta > 0.0 then (
		  let b = sqrt delta in
		  let first = ((-. a) -. b) /. denom
		  and second = ((-. a) +. b) /. denom in
		  let choose dist =
		    if dist < best_t then (
		      let opacity' = 1.0 -.
                        (Material.get_property
			   ~material:(get_object_material obj)
			   ~property:`TRANSPARENCY) in
		      (dist, Sphere s, opacity +. opacity')
		    ) else (
		      (best_t, best_obj, opacity)
		    )
		  in
		    if first > minimal_distance then (
		      if second > minimal_distance then (
                        let closer = if first < second then first else second in
			  choose closer
		      ) else (
                        choose first
		      )
		    ) else (
		      if second > minimal_distance then (
                        choose second
		      ) else (
                        (best_t, best_obj, opacity)
		      )
		    );

                )else
		  (best_t, best_obj, opacity)

	  | _ -> (best_t, best_obj, opacity)
      in
        List.fold_left collide (infinity, NoObject, 0.0) !objects
  end

module Graph =
  struct
    let start x y =
      (* Open graphics *)
      Graphics.open_graph "";
      Graphics.resize_window x y

    let put_pixel x y color =
      let c = Color.graph_of_color color in
        Graphics.set_color c;
        Graphics.plot x y

    let stop () =
      Graphics.close_graph ()

    let wait () =
      ignore (input_char stdin)
  end

module Render =
  struct
    let n_vacuum = 1.0
    let n_air = 1.0002926
    let n_water = 1.333
    let n_diamond = 2.419
    let n_amber = 1.55
    let n_salt = 1.544
    let n_ice = 1.31
    let n_glass = 1.60

    (* Stats *)
    let main_rays = ref 0
    and reflect_rays = ref 0
    and refract_rays = ref 0
    and shadow_rays = ref 0

    (* Arguments:
       ambient - Ambient color
       refract_stack - Stack of refract indexes used for leaving objects
       depth - Current depth used to stop recursion at some level
       ray - currently traced ray
    *)
    let rec tracer ~background ~ambient ~refract_stack ~depth ~ray =
      let t, obj, _ = Scene.find_collision ~ray in
        if t < infinity then (
	  (* We've got a collision *)
	  let collision_point = Ray.point_of_t ~ray ~t in
	  let normal = Scene.get_normal obj collision_point in

	  (* Read color info at collision point *)

	  let helper = Scene.get_object_color ~obj ~point:collision_point in
	  let diffuse = helper ~texture:`DIFFUSE
	  and specular = helper ~texture:`SPECULAR
	  and mirror = helper ~texture:`MIRROR in


	  (* Read other material properties *)
	  let get_prop prop =
	    Material.get_property
	      ~material:(Scene.get_object_material obj)
	      ~property:prop
	  in
	  let shininess = get_prop `SHININESS
	  and transparency = get_prop `TRANSPARENCY
	  and obj_n = get_prop `N in

	  (* Create reflect ray as it's angle is
	     needed for shadow calculations *)
	  let reflect_ray =
	    Ray.gen_reflected_ray
	      ~ray
	      ~collision_point
	      ~normal
	  in

	  (* Function to be 'left folded' over lights list
	     it checks shadow rays for diffuse + specular lightening *)
	  let light_check (diffuse, spec) light =
	    let light_pos = Scene.get_light_position light
	    and light_color = Scene.get_light_color light in
	    let light_ray =
	      Ray.ray_of_points
                ~source:collision_point
                ~destination:light_pos ()
	    in
	      incr  shadow_rays;
	      (* Is anything in a way? *)
	      (* FIXME:
		 In this place we should add additional light filter so the light
		 would get coloured when passing through transparent object
		 It would have to be done in the Scene collision function...
	      *)
	      let distance, obj, opacity =
                Scene.find_collision ~ray:light_ray in
	      let light_visibility =
                let crop a = if a < 0.0 then 0.0 else a in
                if distance = infinity then
		  1.0
                else
		  crop (0.6 -. opacity)
	      in
	      let light_dir = Ray.get_dir light_ray
	      and reflect_dir = Ray.get_dir reflect_ray in
	      let diffuse_coeff =
                light_visibility *. Vect.dot normal light_dir
	      and specular_coeff =
                light_visibility *. Vect.dot reflect_dir light_dir in
	      let specular_coeff =
                if specular_coeff > 0.0 then
		  specular_coeff ** shininess
                else 0.0
	      in
                (* Take in account computations for previous lights
		   and light colour *)
	      let diffuse' =
                Color.add [
		  diffuse;
		  Color.mul diffuse_coeff light_color
                ]in
	      let spec' =
                Color.add [
		  spec;
		  Color.mul specular_coeff light_color
                ]in
                (diffuse', spec');
	  in

	  (* fold this function and calculate diffuse and specular light intensity *)
	  let diffuse_color, specular_color  =
	    Scene.lights_fold light_check (Color.black, Color.black)

	  and refract_color =
	    if transparency = 0.0 || depth = 0 then (
	      Color.black
	    ) else (
	      let refract_stack' = Stack.copy refract_stack in
	      let cur_n = Stack.top refract_stack' in
	      let dot_product = Vect.dot ray.Ray.dir normal in
	      let leaving = if dot_product > 0.0 then true else false in
                (* Accoring to leaving set choose new 'n' and normal *)
	      let normal =
                if leaving then
		  Vect.sub Vect.zero normal
                else
		  normal
	      in
	      let new_n =
                if leaving = true then (
		  Stack.pop refract_stack'
                )else (
		  Stack.push obj_n refract_stack';
		  obj_n
                )
	      in
	      let refract_ray  =
                Ray.gen_refracted_ray
		  ~ray
		  ~collision_point
		  ~normal
		  ~cur_n
		  ~new_n
	      in
                incr refract_rays;
                let result =
		  tracer
		    ~ambient
		    ~background
		    ~refract_stack:refract_stack'
		    ~depth:(depth - 1)
		    ~ray:refract_ray
                in
                let color =
		  match result with
		    | `BACKGROUND -> background (* Color.black*)
		    | `COLOR c -> c
                in
		  Color.mul transparency color
	    )
	  and reflect_color =
	    if depth = 0 || Color.is_zero mirror then
	      Color.black
	    else (
	      incr reflect_rays;
	      let result =
                tracer
		  ~ambient
		  ~background
		  ~refract_stack:(Stack.copy refract_stack)
		  ~depth:(depth - 1)
		  ~ray:reflect_ray
	      in
                match result with
		  | `BACKGROUND -> (*Color.black *) background
		  | `COLOR c -> c
	    )
	  in
	  let one = Color.combine (Color.add [ambient; diffuse_color]) diffuse
	  and two = Color.combine reflect_color mirror
	  and three = Color.combine specular_color specular
	  and four = refract_color in
	  let result = Color.add [one; two; three; four] in
	    `COLOR result
        )else (
	  `BACKGROUND
        )

    let ray_iter
        ~antialiasing
        ~background
        ~put_pixel
        ~xres ~yres
        ~camera
        ~tracer =
      let trace_for_color ray =
        match tracer ~ray with
	  | `BACKGROUND -> background
	  | `COLOR c -> c
      in
        if antialiasing then (
	  let ray_of_xy =
	    Camera.gen_ray_of_xy
	      ~cnt:main_rays
	      ~xres:(xres * 5)
	      ~yres:(yres * 5) ~camera
	  in
	    for y = 0 to yres do
	      for x = 0 to xres do
                let curx, cury = x * 5 + 2, y * 5 + 2 in
                let rays = [
		  ray_of_xy (curx-1) (cury-1);
		  ray_of_xy (curx+1) (cury+1);
		  ray_of_xy (curx-1) (cury+1);
		  ray_of_xy (curx+1) (cury-1);
                ]in
                let colors = List.map trace_for_color rays in
		  put_pixel x y (Color.average colors)
	      done;
	    done
        )else (
	  let ray_of_xy =
	    Camera.gen_ray_of_xy
	      ~cnt:main_rays
	      ~xres ~yres
	      ~camera
	  in
	    for y = yres downto 0 do
	      for x = 0 to xres do
                let ray = ray_of_xy x y in
                let color = trace_for_color ray in
		  put_pixel x y color
	      done;
	    done
        )
    ;;

    let render
        ?(n_atmosphere=n_air)
        ?(antialiasing=true)
        ?(background=Color.black)
        ?(ambient=Color.black)
        ~xres ~yres ~camera () =

      printf "*** Rendering scene ***\n";
      let time1 = Unix.time () in
      Camera.print camera;

      Graph.start xres yres;
      let refract_stack = Stack.create() in
        Stack.push n_atmosphere refract_stack;

        ray_iter
	  ~antialiasing
	  ~background
	  ~xres ~yres
	  ~camera
	  ~put_pixel:Graph.put_pixel
	  ~tracer:(tracer ~background ~ambient ~refract_stack ~depth:8);

        printf "(%d main rays) (%d reflect) (%d refract) (%d shadow rays) "
	  !main_rays !reflect_rays !refract_rays !shadow_rays;
        printf "traced = %d in %5.2f seconds\n%!"
	  (!main_rays + !reflect_rays + !refract_rays + !shadow_rays)
	  (Unix.time () -. time1);

        Graph.wait ();
        Graph.stop ();
  end


type renderer =
    ?background:Color.t
    -> ?ambient:Color.t
    -> camera:Camera.t
    -> unit -> unit

let render_scene1 (renderer:renderer) =
  (* Textures *)
  let c_checked1 =
    Texture.create
      ~pattern:(`CHECKED (Color.black, Color.white))
      ~sizex:1.0 ~sizey:1.0 ()

  and c_checked2 =
    Texture.create
      ~pattern:(`CHECKED (Color.green, Color.yellow))
      ~sizex:0.1 ~sizey:0.1 ()

  and c_gradient =
    Texture.create
      ~pattern:(`XGRADIENT (Color.red, Color.yellow)) ()
  in

  (* Materials *)
  let m_floor =
    Material.create
      ~specular:Texture.black
      ~mirror:Texture.black
      ~diffuse:c_checked1 ()

  and m_gradient =
    Material.create
      ~diffuse:c_gradient ()

  and m_blue_checked =
    Material.create
      ~diffuse:c_checked2 ()

  in
    (* Objects *)
    Scene.add_plane
      ~distance:(-.1.0)
      ~normal:(Vect.create 0. 1. 0.)
      ~material:m_floor;

    Scene.add_sphere
      ~location:(Vect.create 0.5 0. (8.))
      ~radius:1.0
      ~material:m_blue_checked;

    Scene.add_sphere
      ~location:(Vect.create (-.0.7) (-.0.5) (7.))
      ~radius:0.5
      ~material:m_gradient;

    Scene.add_area_light
      ~location:(Vect.create 3. 12. 0.)
      ~radius:2.
      ~color:(Color.create 0.03 0.03 0.03) ~lights:20;

    let camera = Camera.create ()
    and ambient = Color.create 0.1 0.1 0.1
    and background = Color.create 0.5 0.5 0.5 in
      renderer ~ambient ~background ~camera ()
;;

let render_scene2 (renderer : renderer) =
  (* Textures *)
  let tex_checked =
    Texture.create
      ~pattern:(`CHECKED
		  (
		    Color.create 0.0 0.0 0.3 ,
		    Color.create 0.9 0.9 0.9
		  )
	       )
      ~sizex:0.70
      ~sizey:0.70 ()

  and tex_checked2 =
    Texture.create
      ~pattern:(`CHECKED (Color.green, Color.yellow))
      ~sizex:0.1
      ~sizey:0.1 ()
  in

  (* Materials *)
  let m_floor =
    Material.create
      ~specular:Texture.black
      ~mirror:Texture.black
      ~diffuse:tex_checked ()

  and m_big_ball =
    Material.create
      ~diffuse:tex_checked2
      ~mirror:(Texture.create_plain 0.2 0.2 0.2) ()

  and m_glass =
    Material.create
      ~diffuse:(Texture.create_plain 0.1 0.1 0.1)
      ~specular:(Texture.create_plain 0.5 0.5 0.5)
      ~mirror:(Texture.create_plain 0.08 0.08 0.08)
      ~shininess:5.0
      ~n:(Render.n_glass)
      ~transparency:0.9 ()
  in
    (* Objects *)
    Scene.add_plane
      ~distance:(-.1.0)
      ~normal:(Vect.create 0. 1. 0.)
      ~material:m_floor;

    Scene.add_sphere
      ~location:(Vect.create 0.5 0. (10.))
      ~radius:1.0
      ~material:m_big_ball;

    Scene.add_sphere
      ~location:(Vect.create (-.0.7) (-.0.5) (7.))
      ~radius:0.5
      ~material:m_glass;

    Scene.add_sphere
      ~location:(Vect.create (0.7) (-.0.5) (8.))
      ~radius:0.9
      ~material:m_glass;

    Scene.add_area_light
      ~location:(Vect.create 3. 12. 0.)
      ~radius:2.
      ~color:(Color.create 0.03 0.03 0.03)
      ~lights:20;

    let camera =
      Camera.create
        ~loc:(Vect.create (-.2.8) 3.0 (1.2))
        ~dir:(Vect.create 0.4 (-.0.45) 1.)
        ~auto_top:true
        ~fov:(Camera.fov_of_degree 60.0)
        ~ratio:(4.0/.3.0)
        ()
    and ambient = Color.create 0.1 0.1 0.1
    and background = Color.create 0.7 0.7 1.0 in
      renderer ~ambient ~background ~camera ()
;;

let render_scene_with_diamond (renderer : renderer) =
  (* Textures *)
  let tex_checked =
    Texture.create
      ~pattern:(`CHECKED
		  (Color.create 0.0 0.0 0.3 ,
		   Color.create 0.9 0.9 0.9)
	       )
      ~sizex:0.70
      ~sizey:0.70 ()

  and tex_checked2 =
    Texture.create
      ~pattern:(`CHECKED (Color.green, Color.yellow))
      ~sizex:0.1
      ~sizey:0.1 ()
  in

  (* Materials *)
  let m_floor =
    Material.create
      ~specular:Texture.black
      ~mirror:Texture.black
      ~diffuse:tex_checked ()

  and m_big_ball =
    Material.create
      ~diffuse:tex_checked2
      ~mirror:(Texture.create_plain 0.0 0.0 0.0) ()

  and m_glass =
    Material.create
      ~diffuse:(Texture.black)
      ~specular:(Texture.create_plain 0.0 0.0 0.0)
      ~mirror:(Texture.create_plain 0.00 0.00 0.00)
      ~shininess:5.0
      ~n:(Render.n_glass)
      ~transparency:1.0 ()

  and m_diamond =
    Material.create
      ~diffuse:(Texture.black)
      ~specular:(Texture.create_plain 0.0 0.0 0.0)
      ~mirror:(Texture.create_plain 0.5 0.5 0.5)
      ~shininess:5.0
      ~n:(Render.n_diamond)
      ~transparency:0.9 ()

  in
    (* Objects *)
    Scene.add_plane
      ~distance:(-.1.0)
      ~normal:(Vect.create 0. 1. 0.)
      ~material:m_floor;

    Scene.add_sphere
      ~location:(Vect.create 0.0 0. 6.)
      ~radius:1.0
      ~material:m_glass;

    Scene.add_sphere
      ~location:(Vect.create 0.0 0.0 6.0)
      ~radius:0.75
      ~material:m_diamond;

    Scene.add_area_light
      ~location:(Vect.create 3. 12. 0.)
      ~radius:2.
      ~color:(Color.create 0.03 0.03 0.03)
      ~lights:20;

    let camera = Camera.create ()
    and ambient = Color.create 0.1 0.1 0.1
    and background = Color.create 0.7 0.7 1.0 in
      renderer ~ambient ~background ~camera ()
;;


let render_scene_billiards (renderer : renderer) =
  (* Textures *)
  let tex_checked =
    Texture.create
      ~pattern:(`CHECKED
		  (Color.create 0.0 0.0 0.3 ,
		   Color.create 0.9 0.9 0.9)
	       )
      ~sizex:0.70
      ~sizey:0.70 ()

  and tex_checked2 =
    Texture.create
      ~pattern:(`CHECKED (Color.green, Color.yellow))
      ~sizex:0.1
      ~sizey:0.1 ()
  in

  (* Materials *)
  let m_floor =
    Material.create
      ~specular:Texture.black
      ~mirror:Texture.black
      ~diffuse:tex_checked ()

  and m_redball =
    Material.create
      ~diffuse:tex_checked2
      ~mirror:(Texture.black)
      ~transparency:0.0 ()

  and m_glass =
    Material.create
      ~diffuse:(Texture.create_plain 0.1 0.1 0.1)
      ~specular:(Texture.create_plain 0.5 0.5 0.5)
      ~mirror:(Texture.create_plain 0.08 0.08 0.08)
      ~shininess:5.0
      ~n:(Render.n_glass)
      ~transparency:0.0 ()
  in

  let rand_texture () =
    let color () =
      match Random.int 10  with
        |0 -> Color.white
        |1 -> Color.black
        |2 -> Color.green
        |3 -> Color.red
        |4 -> Color.blue
        |5 -> Color.create 1.0 1.0 0.0
        |6 -> Color.create 0.0 1.0 1.0
        |7 -> Color.create 1.0 0.0 1.0
        |8 -> Color.create 0.5 0.5 0.5
        |_ ->
	    Color.create
	      (Random.float 1.0)
	      (Random.float 1.0)
	      (Random.float 1.0)
    in
      match Random.int 2 with
        |0 ->
	    Texture.create ~pattern:(`PLAIN (color ())) ()
        |_ ->
	    Texture.create
	      ~pattern:(`CHECKED (color (), color ()))
	      ~sizex:0.1
	      ~sizey:0.1 ()
  in

  let create_spheres st_x st_z x_cnt z_cnt y radius =
    let left = st_x -. (float_of_int x_cnt) *. radius
    and top = st_z -. (float_of_int z_cnt) *. radius
    in
      for x = 0 to x_cnt-1 do
        for z = 0 to z_cnt-1 do
	  let material =
	    Material.create
	      ~diffuse:(rand_texture ())
	      ~mirror:(Texture.create_plain 0.0 0.0 0.0)
	      ~transparency:0.8
	      ~n:Render.n_glass ()
	  in
	  let center_x = left +. (float_of_int x) *. radius *. 2.0
	  and center_z = top +. (float_of_int z) *. radius *. 2.0
	  in
	    Scene.add_sphere
	      ~location:(Vect.create center_x y center_z)
	      ~radius:(radius -. 0.01)
	      ~material
        done
      done
  in

    (* Objects *)
    Scene.add_plane
      ~distance:(-.0.3)
      ~normal:(Vect.create 0. 1. 0.)
      ~material:m_floor;

    create_spheres 0.0 4.0 6 6 0.0 0.3;
    create_spheres 0.0 4.0 5 5 0.6 0.3;
    create_spheres 0.0 4.0 4 4 1.2 0.3;
    create_spheres 0.0 4.0 3 3 1.8 0.3;
    create_spheres 0.0 4.0 2 2 2.4 0.3;
    create_spheres 0.0 4.0 1 1 3.0 0.3;

    Scene.add_light
      ~location:(Vect.create (-.5.) 5. 0.)
      ~color:(Color.create 0.3 0.3 0.3);
    Scene.add_area_light
      ~location:(Vect.create 3. 12. 0.)
      ~radius:2.
      ~color:(Color.create 0.03 0.03 0.03)
      ~lights:20;

    let camera =
      Camera.create
        ~loc:(Vect.create (-.3.0) 7.8 (-5.0))
        ~dir:(Vect.create 0.4 (-.0.86) 1.)
        ~auto_top:true
        ~fov:(Camera.fov_of_degree 60.0)
        ~ratio:(4.0/.3.0)
        ()
    and ambient = Color.create 0.1 0.1 0.1
    and background = Color.create 0.7 0.7 1.0 in
      renderer ~ambient ~background ~camera ()

;;

let _ =
  let renderer =
    Render.render
      ~n_atmosphere:Render.n_air
      ~antialiasing:true
      ~xres:640
      ~yres:480
  in
(*    render_scene_billiards renderer; *)
	render_scene1 renderer;
;;
