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

module Quat =
  struct
    type t = {
      x : float;
      y : float;
      z : float;
      w : float;
    }

    let zero = {
      x = 0.0;
      y = 0.0;
      z = 0.0;
      w = 0.0
    }

    let print v =
      printf "(%5.7f %5.7f %5.7f %5.7f)" v.x v.y v.z v.w

    let create x y z w = {
      x = x;
      y = y;
      z = z;
      w = w;
    }

    let get v =
      (v.x, v.y, v.z, v.w)

    let add a b = {
      x = a.x +. b.x;
      y = a.y +. b.y;
      z = a.z +. b.z;
      w = a.w +. b.w;
    }

    let sub a b = {
      x = a.x -. b.x;
      y = a.y -. b.y;
      z = a.z -. b.z;
      w = a.w -. b.w;
    }

    let addc ~q ~c = {
      x = q.x +. c;
      y = q.y +. c;
      z = q.z +. c;
      w = q.w +. c;
    }

    let mulc ~q ~c = {
      x = q.x *. c;
      y = q.y *. c;
      z = q.z *. c;
      w = q.w *. c;
    }


    let dot a b =
      (a.x *. b.x) +. (a.y *. b.y) +. (a.z *. b.z) +. (a.w *. b.w)

    let cross a b = {
      x = a.x*.b.x -. a.y*.b.y -. a.z*.b.z -. a.w*.b.w;
      y = a.x*.b.y +. a.y*.b.x +. a.z*.b.w -. a.w*.b.z;
      z = a.x*.b.z -. a.y*.b.w +. a.z*.b.x +. a.w*.b.y;
      w = a.x*.b.w +. a.y*.b.z -. a.z*.b.y +. a.w*.b.x;
    }

    let square a = {
      x = a.x *. a.x -. a.y *. a.y -. a.z -. a.w *. a.w;
      y = 2. *. a.x *. a.y; 
      z = 2. *. a.x *. a.z; 
      w = 2. *. a.x *. a.w; 
    }

    let len2 a =
      a.x *. a.x +. a.y *. a.y +. a.z *. a.z +. a.w *. a.w

    let len a =
      sqrt (len2 a)

    let normalize a =
      let len = len a in {
	  x = a.x /. len;
	  y = a.y /. len;
	  z = a.z /. len;
	  w = a.w /. len;
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
        ?(loc=Vect.create 0. 0. (-7.))
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

module Julia =
  struct

    let pt_debug a =
      let a,b,c,d = a in
	printf "(%10.5f %10.5f %10.5f %10.5f)" a b c d

    let create () =
      Quat.create (-1.) (-0.2) 0.0 0.0

    let intersect ~julia ~ray =
      (* Initial ray start position *)
      let dir = ray.Ray.dir
      and start = ray.Ray.start
      and max = 60.0
      and eps = 0.1 in

      let rec limes ~zn ~dn ~iter =
	let zn' = Quat.add (Quat.cross zn zn) julia
	and dn' = Quat.mulc ~q:(Quat.cross zn dn) ~c:2.0 in
	let len = Quat.len2 zn' in
	  if iter = 0 or len > max then
	    len, dn'
	  else
	    limes ~zn:zn' ~dn:dn' ~iter:(iter - 1)
      in

      let rec loop prev_dist point =
	(* point - point being tested for distance from julia set
	 * d0 - derivative start *)
	let d0 = Quat.create 1. 0. 0. 0. in

	let len2, dn = limes ~zn:point ~dn:d0 ~iter:30 in
	let len = sqrt len2 in  
	let dist = 0.5 *. len *. (log len) /. (Quat.len dn) in
 	let dx,dy,dz = Vect.get dir in
	let point' = Quat.add point (Quat.create (dist*.dx) (dist*.dy) (dist *.dz) 0.)
	in

	  if prev_dist < dist then (
	    point, prev_dist
	  ) else if dist < eps then (
	    point', dist
	  ) else (
	    loop dist point'
	  )
      in
      

      let z0 = Quat.create start.Vect.x start.Vect.y start.Vect.z 0.0 in
	loop infinity z0

    (* Gradient normal *)
    let normal ~julia ~point =
      let d = 0.0001
      and max = 4  in
      let dd = [|
	Quat.add point (Quat.create d 0. 0. 0.);
	Quat.add point (Quat.create (-.d) 0. 0. 0.);
	Quat.add point (Quat.create 0. d 0. 0.);
	Quat.add point (Quat.create 0. (-.d) 0. 0.);
	Quat.add point (Quat.create 0. 0. d 0.);
	Quat.add point (Quat.create 0. 0. (-.d) 0.);
      |] in
      let step i a = dd.(i) <- Quat.add (Quat.cross a a) julia in
      let rec loop iter =
	Array.iteri step dd;
	if iter <> 0 then loop (iter - 1)
      in
	loop max;
	let g_x = (Quat.len dd.(0)) -. (Quat.len dd.(1)) 
	and g_y = (Quat.len dd.(2)) -. (Quat.len dd.(3)) 
	and g_z = (Quat.len dd.(4)) -. (Quat.len dd.(5)) in
(*	  pt_debug dd.(0);
	  pt_debug dd.(1);
	  printf "Got %10.5f %10.5f %10.5f\n" g_x g_y g_z; *)
	  (g_x, g_y, g_z)

  end


module Scene =
  struct
    let pi = 4.0 *. atan(1.0)

    type light = {
      light_pos : Vect.t;
      light_color : Color.t;
    }

    let lights = ref []
    let julia = Julia.create ()
    let julia_color = Color.red

    (* Getters *)
    let get_object_color () = julia_color

    let get_light_position l =
      l.light_pos

    let get_light_color l =
      l.light_color

    let get_normal obj point =
      match obj with
        |_ -> failwith "Wrong object specified"

    (* Adders *)
    let add_light ~location ~color =
      let light = {
        light_pos = location;
        light_color = color;
      } in
        lights := light :: (!lights)

    let lights_fold f init =
      List.fold_left f init !lights

    let rec find_collision ~ray =
      flush stdout;
      let point, dist = Julia.intersect ~julia ~ray in
      let x,y,z,w = Quat.get point in
      let col = Vect.create x y z in
	if dist < 0.1 then
	  let nx, ny, nz = Julia.normal ~julia ~point in
	    (col, dist, Vect.normalize (Vect.create nx ny nz))
	else 
	  (col, dist, col) (* PLACE HERE A NORMAL! *)

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
    and shadow_rays = ref 0

    (* Arguments:
       ambient - Ambient color
       refract_stack - Stack of refract indexes used for leaving objects
       depth - Current depth used to stop recursion at some level
       ray - currently traced ray
    *)
    let rec tracer ~background ~ambient ~depth ~ray =
      let collision_point, t, normal = Scene.find_collision ~ray in
        if t < 0.1 then ( (* EPS! *)
	  (* Read color info at collision point *)

	  let diffuse = Scene.get_object_color () in

	  (* Function to be 'left folded' over lights list
	     it checks shadow rays for diffuse + specular lightening *)
	  let light_check diffuse light =
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

	      let point, distance, _ =
                Scene.find_collision ~ray:light_ray in
	      let light_visibility = 1.0
(*                if distance > 0.1 then
		  1.0
                else
		  0.0 *)
	      in
	      let light_dir = Ray.get_dir light_ray in
	      let diffuse_coeff =
                light_visibility *. Vect.dot normal light_dir
	      in
                (* Take in account computations for previous lights
		   and light colour *)
                Color.add [
		  diffuse;
		  Color.mul diffuse_coeff light_color
                ]
	  in

	  (* fold this function and calculate diffuse and specular light intensity *)
	  let diffuse_color =
	    Scene.lights_fold light_check Color.black in

	  let result = Color.combine (Color.add [ambient; diffuse_color]) diffuse in
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

      ray_iter
	~antialiasing
	~background
	~xres ~yres
	~camera
	~put_pixel:Graph.put_pixel
	~tracer:(tracer ~background ~ambient ~depth:8);

      printf "(%d main rays) (%d shadow rays) "
	!main_rays !shadow_rays;
      printf "traced = %d in %5.2f seconds\n%!"
	(!main_rays + !shadow_rays)
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

  Scene.add_light
    ~location:(Vect.create 3. 8. (-7.))
    ~color:(Color.create 0.3 0.3 0.3);


  let camera = Camera.create 
    ~loc:(Vect.create (0.0) 0.0 (-7.0)) 
    ~dir:(Vect.create 0.0 0.0 1.0)
    ()
  and ambient = Color.create 0.1 0.1 0.1
  and background = Color.create 0.5 0.5 0.5 in
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
