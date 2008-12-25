 (* Tiny Objective Caml Raytracer	*)
(* (C) 2008 by Tomasz bla Fortuna	*)
(* License: GPLv3+			*)

open Printf

let fabs a = if a < 0.0 then -. a else a

module Cfg =
  struct
    let eps = 0.0005
    let max_iter = 5
    let max_inf = 20.0
    let delta = 0.0001
    let bounding_sphere = 100.0
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

    let len2 a =
      a.x *. a.x +. a.y *. a.y +. a.z *. a.z

    let len a =
      sqrt (len2 a)

    let normalize a =
      let len = len a in {
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

    let fromvect ?(w=0.0) v =
      let x,y,z = Vect.get v in
	create x y z w

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

    let mul a b = {
      x = a.x*.b.x -. a.y*.b.y -. a.z*.b.z -. a.w*.b.w;
      y = a.x*.b.y +. a.y*.b.x +. a.z*.b.w -. a.w*.b.z;
      z = a.x*.b.z -. a.y*.b.w +. a.z*.b.x +. a.w*.b.y;
      w = a.x*.b.w +. a.y*.b.z -. a.z*.b.y +. a.w*.b.x;
    }

    let cross a b c = {
      x = a.y*.(b.w*.c.z -. b.z*.c.w) +. b.y*.(a.z*.c.w -. a.w*.c.z) +. c.y*.(a.w*.b.z -. a.z*.b.w);
      y = a.x*.(b.z*.c.w -. b.w*.c.z) +. b.x*.(a.w*.c.z -. a.z*.c.w) +. c.x*.(a.z*.b.w -. a.w*.b.z);
      z = a.x*.(b.w*.c.y -. b.y*.c.w) +. b.x*.(a.y*.c.w -. a.w*.c.y) +. c.x*.(a.w*.b.y -. a.y*.b.w);
      w = a.x*.(b.y*.c.z -. b.z*.c.y) +. b.x*.(a.z*.c.y -. a.y*.c.z) +. c.x*.(a.y*.b.z -. a.z*.b.y);
    }

    let square a = {
      x = a.x*.a.x -. a.y*.a.y -. a.z*.a.z -. a.w*.a.w;
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

    let ray_of_points ~source ~destination =
      let dir = Vect.sub destination source in
        create ~start:source ~dir:(Vect.normalize dir) ()

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
    type t = {
      color : Color.t;
      c : Quat.t;
    }

    let create () = {
      color = Color.red;
      c = Quat.create (-1.) (-0.2) 0.0 0.0;
    }

    let get_color ~julia = julia.color
    let get_c ~julia = julia.c

    let step ~z ~c =
      Quat.add (Quat.square z) c

    let intersect ~julia ~ray =
      printf "Intersecting julia with ray ";
      Ray.print ray;
      printf "\n";


      let rec limes ~zn ~dn ~iter =
	let zn' = step ~z:zn ~c:julia.c
	and dn' = Quat.mulc ~q:(Quat.mul zn dn) ~c:2.0 in
	let len2 = Quat.len2 zn' in
(*	  printf "Limes point=";
	  Quat.print zn';
	  printf " %d iterleft \n" iter; *)
	  if iter = 0 (* or len2 > Cfg.max_inf *)then
	    len2, dn'
	  else
	    limes ~zn:zn' ~dn:dn' ~iter:(iter - 1)
      in

      let rec loop t =
	(* point - point being tested for distance from julia set
	 * d0 - derivative start *)
	let point = Quat.fromvect (Ray.point_of_t ~ray ~t) in
(*	  printf "Point = ";
	  Quat.print point;
	  printf "; \n"; *)

	  if (Quat.len2 point) > Cfg.bounding_sphere then (
	    printf "Out of sphere!\n";

	    point, infinity
	  ) else (	    

	    let d0 = Quat.create 1. 0. 0. 0. in
	    let len2, dn = limes ~zn:point ~dn:d0 ~iter:Cfg.max_iter in
	    let len = sqrt len2 in
	    let dist = 0.5 *. (log len) *. len /. (Quat.len dn) in
	    let t' = t +. dist in
	      printf "dist=%10.5f " dist;
	      if dist < Cfg.eps then (
		printf "Found!\n";
		point, dist
	      ) else (
		printf "Looping...\n";
		loop t'
	      )
          )
      in
	loop 0.0

    (* Gradient normal *)
    let normal ~julia ~point =
      let d = Cfg.delta in
      let dd = [|
	Quat.add point (Quat.create d 0. 0. 0.);
	Quat.add point (Quat.create (-.d) 0. 0. 0.);
	Quat.add point (Quat.create 0. d 0. 0.);
	Quat.add point (Quat.create 0. (-.d) 0. 0.);
	Quat.add point (Quat.create 0. 0. d 0.);
	Quat.add point (Quat.create 0. 0. (-.d) 0.);
      |] in
      let step i z = dd.(i) <- step ~z ~c:julia.c in
      let rec loop iter =
	Array.iteri step dd;
	if iter <> 0 then loop (iter - 1)
      in
	loop Cfg.max_iter;
	let g_x = (Quat.len dd.(0)) -. (Quat.len dd.(1)) 
	and g_y = (Quat.len dd.(2)) -. (Quat.len dd.(3)) 
	and g_z = (Quat.len dd.(4)) -. (Quat.len dd.(5)) in
(*	  pt_debug dd.(0);
	  pt_debug dd.(1);
	  printf "Got %10.5f %10.5f %10.5f\n" g_x g_y g_z; *)
	  (g_x, g_y, g_z)

  end


module Render =
  struct
    (* Stats *)
    let main_rays = ref 0
    and shadow_rays = ref 0

    (* The main tracing program created in render *)
    let rec tracer ~background ~ambient ~depth ~lights ~julia ~ray =
      (* Find collision point with julia *)
      let point, t = Julia.intersect ~julia ~ray in
      let x, y, z, _ = Quat.get point in 
      let collision_point = Vect.create x y z in
        if t < Cfg.eps then ( (* EPS! *)
	  (* Calculate normal *)
	  let nx, ny, nz = Julia.normal ~julia ~point in
	  let normal = Vect.normalize (Vect.create nx ny nz) in
	  (* Read color info at collision point *)

	  let diffuse = Julia.get_color ~julia in

	  (* Function to be 'left folded' over lights list
	     it checks shadow rays for diffuse + specular lightening *)
	  let light_check diffuse light =
	    let light_pos, light_color = light in
	    let light_ray =
	      Ray.ray_of_points
                ~source:collision_point
                ~destination:light_pos
	    in 
	      incr shadow_rays; 

(*	      let point, distance, _ =
                Scene.find_collision ~ray:light_ray in *)
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
	    List.fold_left light_check Color.black lights in


	  let result = Color.combine (Color.add [ambient; diffuse_color]) diffuse in
	    `COLOR result
        )else (
	  `BACKGROUND
        )


    let render
        ?(background=Color.black)
        ?(ambient=Color.black)
        ~xres ~yres ~camera ~lights ~julia () =

      let ray_iter ~put_pixel ~tracer =
	let trace_for_color ray =
          match tracer ~ray with
	    | `BACKGROUND -> background
	    | `COLOR c -> c
	in
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
      in

	printf "*** Rendering scene ***\n";
	let time1 = Unix.time () in

	  Camera.print camera;
	  Graph.start xres yres;

	  ray_iter ~put_pixel:Graph.put_pixel ~tracer:(tracer ~background ~ambient ~lights ~julia ~depth:8);
	  
	  printf "(%d main rays) (%d shadow rays) "
	    !main_rays !shadow_rays;
	  printf "traced = %d in %5.2f seconds\n%!"
	    (!main_rays + !shadow_rays)
	    (Unix.time () -. time1);
	  
	  Graph.wait ();
	  Graph.stop ();
  end


(* Scene settings + renderer settings + render *)
let _ =
  let julia = Julia.create ()
  and lights = [
    (Vect.create 3. 8. (-5.)),
    (Color.create 0.3 0.3 0.3)
  ]
    
  and camera = Camera.create 
    ~loc:(Vect.create (0.0) 0.0 (-6.0)) 
    ~dir:(Vect.create 0.0 0.0 1.0)
    ()
  and ambient = Color.create 0.1 0.1 0.1
  and background = Color.create 0.5 0.5 0.5 in
    
  let test_ray = Ray.ray_of_points 
    ~source:(Vect.create 0.0 0.0 (-9.0))
    ~destination:(Vect.create 0.0 0.0 1.0)
  in
    if false then (
      Julia.intersect ~julia ~ray:test_ray;
      ()
    ) else (
      Render.render ~xres:320 ~yres:240 ~ambient ~background ~camera ~lights ~julia ();
      ()
    )

;;
