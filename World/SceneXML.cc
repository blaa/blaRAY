/**********************************************************************
 * blaRAY -- photon mapper/raytracer
 * (C) 2008 by Tomasz bla Fortuna <bla@thera.be>, <bla@af.gliwice.pl>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * See Docs/LICENSE
 *********************/

#include <stdexcept>
#include <iostream>

#include "World/Scene.hh"

namespace World {
	/** Convert any type to string (debug function )*/
	template<typename T>
	static std::string ToStr(const T &var) {
		std::ostringstream tmp;
		tmp << var;
		return tmp.str();
	}

	/** Convert string to double without checking syntax */
	static Double ToDouble(const std::string &str)
	{
		std::stringstream os(str);
		Double a; /** \bug Nah, should I use sscanf?
			      stringstream is not much predictable */
		os >> a;
		return a;
	}

	/** \brief Exception internal to XML reader */
	class XMLError : public std::runtime_error {
	public:
		/** Create exception with file context */
		XMLError(xmlNodePtr Node, const std::string &Desc)
			: std::runtime_error(
				"On line " + ToStr(xmlGetLineNo(Node)) +
				": " + Desc)
		{
		}

		/** Create exception without file context */
		XMLError(const std::string &Desc)
			: std::runtime_error(Desc)
		{
		}
	};

	/** Check numerical value syntax
	 * \bug Very c-ish code */
	static Bool IsDouble(const std::string &str)
	{
		Bool WasDot = false;
		if (str.length() == 0)
			return false;
		for (UInt i = 0U;
		     i != str.length();
		     i++) {
			if (str[i] == '-' && i == 0)
				continue;

			if (str[i] == '.' && WasDot == false) {
				WasDot = true;
				continue;
			}
			if (str[i] < '0' || str[i] > '9')
				return false;
		}
		return true;
	}

	/** Helper function retrieving property */
	static std::string GetProp(xmlNodePtr Node, const char *id)
	{
		xmlChar *text = (xmlChar *)xmlGetProp(Node,(const xmlChar *)id);
		if (text == NULL)
			return std::string("");
		std::string t((const char *)text);
		xmlFree(text);
		return t;
	}

	/** Read compulsory numerical property and convert to double */
	static Double GetDoubleProp(xmlNodePtr Node, const char *str)
	{
		std::string Prop = GetProp(Node, str);
		if (Prop == "")
			throw XMLError(Node,
			"Unable to find numerical tag " + std::string(str));
		if (!IsDouble(Prop))
		    throw XMLError(Node, "Invalid numerical value");
		Double Val = ToDouble(Prop);
		/* \bug Do some checking! And return exception */
		return Val;
	}

	/** Read optional numerical property and convert to double
	 * if property is not given use the given default value */
	static Double GetDoubleProp(xmlNodePtr Node,
				    const char *str,
				    Double DefaultValue)
	{
		std::string Prop = GetProp(Node, str);
		if (Prop == "")
			return DefaultValue;

		Double Val = ToDouble(Prop);
		/* \bug Do some checking! And return exception */
		return Val;
	}

	/** Ensures given tag name is "Color" */
	static void EnsureColorTag(xmlNodePtr Node, xmlNodePtr ErrorCtx = NULL)
	{
		if (!Node) {
			throw XMLError(ErrorCtx,
				"Expected \"Color\" tag within"
				" this context got nothing ");
		}

		if (xmlStrcmp(Node->name, (const xmlChar *) "Color"))
			throw XMLError(Node,
				"Expected \"Color\" tag got " +
				std::string((char *)Node->name));
	}

	/** Read compulsory ID 
	 * \bug rarely used! 
	 */
	static std::string GetID(xmlNodePtr Node)
	{
		std::string id = GetProp(Node, "id");
		if (id == "")
			throw XMLError(Node,
				"No identifier (id tag) specified");
		return id;
	}

	/** Checks if the current node matches token */
	static Bool IsToken(xmlNodePtr Node, const char *Token)
	{
		if ((xmlStrcmp(Node->name, (const xmlChar *)Token)) == 0)
			return true;
		return false;
	}

	/** Increment token while omitting any commentaries */
	static void OmitComments(xmlNodePtr &Node)
	{
		for (; Node != NULL; Node = Node->next) {
			if (IsToken(Node, "comment"))
				continue;
			break;
		}
	}

	void Scene::DumpLibrary()
	{
		using namespace std;
		cout << "--- Dumping colors: " << endl;
		for (ColIter i = ColMap.begin();
		     i != ColMap.end(); i++) {
			cout << i->first << " == " << i->second << endl;
		}

		cout << "--- Dumping textures: " << endl;
		for (TexIter i = TexMap.begin();
		     i != TexMap.end(); i++) {
			cout << i->first << " == " << *i->second << endl;
		}

		cout << "--- Dumping materials: " << endl;
		for (MatIter i = MatMap.begin();
		     i != MatMap.end(); i++) {
			cout << i->first << " == " << *i->second << endl;
		}
	}

	void Scene::CreateLibrary()
	{
		using namespace std;

		ColMap.insert(make_pair(string("Black"), ColLib::Black()));
		ColMap.insert(make_pair(string("White"), ColLib::White()));
		ColMap.insert(make_pair(string("Red"), ColLib::Red()));
		ColMap.insert(make_pair(string("Green"), ColLib::Green()));
		ColMap.insert(make_pair(string("Blue"), ColLib::Blue()));
		ColMap.insert(make_pair(string("Gray"), ColLib::Gray()));

		TexMap.insert(make_pair(string("Black"), &TexLib::Black()));
		TexMap.insert(make_pair(string("White"), &TexLib::White()));
		TexMap.insert(make_pair(string("Red"), &TexLib::Red()));
		TexMap.insert(make_pair(string("Green"), &TexLib::Green()));
		TexMap.insert(make_pair(string("Blue"), &TexLib::Blue()));
		TexMap.insert(make_pair(string("Gray"), &TexLib::Gray()));

		IdxMap.insert(make_pair(string("Vacuum"), 1.0));
		IdxMap.insert(make_pair(string("Air"), 1.0002926));
		IdxMap.insert(make_pair(string("Water"), 1.333));
		IdxMap.insert(make_pair(string("Diamond"), 2.419));
		IdxMap.insert(make_pair(string("Amber"), 1.55));
		IdxMap.insert(make_pair(string("Salt"), 1.544));
		IdxMap.insert(make_pair(string("Ice"), 1.31));
		IdxMap.insert(make_pair(string("Glass"), 1.60));
	}

	Color Scene::ParseColor(xmlNodePtr Node)
	{
		/* <Color [id="ColorID"] [r="float" g="float" b="float"] />
		 *   If ID is specified:
		 *	if color is is map return it.
		 *	else
		 *		if r,g,b present define color
		 *		else error
		 *	else
		 *	if r,g,b present create and return color
		 *	else error
		*/
		std::string id = GetProp(Node, "id");
		std::string r = GetProp(Node, "r");
		std::string g = GetProp(Node, "g");
		std::string b = GetProp(Node, "b");

		if (id == "" && (r == "" || g == "" || b == ""))
			throw XMLError(Node,
			       "Inappropriate color declaration");

		if (id != "") {
			/* Do we have this id in map? */
			ColIter iter = ColMap.find(id);
			if (iter != ColMap.end())
				return iter->second;

			if (r == "" || g == "" || b == "")
				throw XMLError(Node,
					       "Color \"" + id +
					       "\" doesn't exist in database");
		}

		/* We don't have it - insert it */
		double R, G, B;
		R = ToDouble(r);
		G = ToDouble(g);
		B = ToDouble(b);
		if (R > 1.0 || R < 0.0 || G > 1.0
		    || G < 0.0 || B > 1.0 || B < 0.0)
			throw XMLError(Node,
				"Illegal color attribute value. "
				"R, G, B belongs between 0.0 and 1.0");
		if (id != "")
			ColMap.insert( make_pair(id, Color(R, G, B)) );
		return Color(R, G, B);
	}

	Math::Vector Scene::ParseVector(xmlNodePtr Node)
	{
		std::string x = GetProp(Node, "x");
		std::string y = GetProp(Node, "y");
		std::string z = GetProp(Node, "z");
		if (x == "" || y == "" || z == "")
			throw XMLError(Node,
				"Inappropriate vector declaration");

		double X, Y, Z;
		X = ToDouble(x);
		Y = ToDouble(y);
		Z = ToDouble(z);
		return Math::Vector(X, Y, Z);
	}

	const Texture *Scene::GetTexture(const std::string &id)
	{
		TexIter iter = TexMap.find(id);
		if (iter != TexMap.end())
			return iter->second;
		return NULL;
	}

	const Material *Scene::GetMaterial(const std::string &id)
	{
		MatIter iter = MatMap.find(id);
		if (iter != MatMap.end())
			return iter->second;
		return NULL;
	}


	void Scene::ParseTexture(xmlNodePtr Node)
	{
		std::string id = GetID(Node);
		std::string Type = GetProp(Node, "type");
		if (Type == "")
			throw XMLError(Node,
				       "No texture type specified"
				       " (plane or checked)");
		if (id == "")
			throw XMLError(Node,
				       "No identified given for texture");

		/** \bug Check if texture doesn't exist */

		if (Type == "Plain") {
			xmlNodePtr Child = Node->xmlChildrenNode;
			OmitComments(Child);
			EnsureColorTag(Child, Node);
			Color C = ParseColor(Child);

			Child = Child->next;
			OmitComments(Child);
			if (Child)
				throw XMLError(Node,
					"Plane texture requires"
					" one color parameter");

			TexLib::Plain *Tex = new TexLib::Plain(C);
			/* Add texture to scene so it will be freed
			   and to id list */
			this->AddTexture(Tex);
			TexMap.insert( make_pair(id, Tex) );

		} else if (Type == "Checked") {
			xmlNodePtr Child = Node->xmlChildrenNode;
			OmitComments(Child);
			/* First color */
			EnsureColorTag(Child, Node);
			Color A = ParseColor(Child);

			/* Second color */
			Child = Child->next;
			OmitComments(Child);
			EnsureColorTag(Child, Node);
			Color B = ParseColor(Child);

			Child = Child->next;
			OmitComments(Child);
			if (Child)
				throw XMLError(Node,
					"Checked texture requires"
					" two color parameter");

			/* Additional parameters */
			std::string Width_ =  GetProp(Node, "width");
			std::string Height_ = GetProp(Node, "height");
			std::string Tile_ = GetProp(Node, "tile");
			Double Width, Height;
			Bool Tile = true;
			if (Width_ == "")
				Width = 1.0;
			else	Width = ToDouble(Width_);
			if (Height_ == "")
				Height = 1.0;
			else	Height = ToDouble(Height_);
			if (Tile_ == "false" || Tile_ == "no")
				Tile = false;

			TexLib::Checked *Tex =
				new TexLib::Checked(A, B, Width, Height, Tile);
			this->AddTexture(Tex);

			TexMap.insert( make_pair(id, Tex) );
		} else
			throw XMLError(Node,
				"Invalid texture type specified");
	}

	Double Scene::ParseIdx(xmlNodePtr Node)
	{
		std::string idx = GetProp(Node, "idx");
		if (IsDouble(idx)) {
			Double idx = GetDoubleProp(Node, "idx");
			return idx;
		} else {
			if (idx != "") {
				IdxIter iter = IdxMap.find(idx);
				if (iter == IdxMap.end())
					throw XMLError(Node,
						       "No refractive index"
						       " with that name");
				return iter->second;
			} else {
				return MatLib::IdxGlass;
			}
		}
	}

	void Scene::ParseMaterial(xmlNodePtr Node)
	{
		std::string id = GetProp(Node, "id");
		if (id == "")
			throw XMLError("Material identifier not specified");
		if (GetMaterial(id) != NULL)
			throw XMLError("Material already defined");

		/* Material parameters with default values */
		const Texture
			*Diffuse = &TexLib::Green(),
			*Specular = &TexLib::White(),
			*Reflect = &TexLib::Black(),
			*Refract = &TexLib::Black();

		std::string diffuse = GetProp(Node, "diffuse");
		std::string specular = GetProp(Node, "specular");
		std::string reflect = GetProp(Node, "reflect");
		std::string refract = GetProp(Node, "refract");
		Double Shininess = GetDoubleProp(Node, "shininess", 12.0);
		Double Idx = ParseIdx(Node);

		/* Decode given non-default textures */
		if (diffuse != "") Diffuse = GetTexture(diffuse);
		if (specular != "") Specular = GetTexture(specular);
		if (reflect != "") Reflect = GetTexture(reflect);
		if (refract != "") Refract = GetTexture(refract);

		if (!Diffuse) throw XMLError(Node,
					     "Undefined texture " + diffuse);
		if (!Specular) throw XMLError(Node,
					      "Undefined texture " + specular);
		if (!Reflect) throw XMLError(Node,
					     "Undefined texture " + reflect);
		if (!Refract) throw XMLError(Node,
					     "Undefined texture " + refract);

		/* Everything read, insert to library */
		Material *Mat = new Material(
			*Diffuse, *Specular,
			*Refract, *Reflect,
			0.0, 0.0, 0.0,
			Shininess, Idx);

		MatMap.insert( make_pair(id, Mat) );
		this->AddMaterial(Mat);
	}

	void Scene::ParseLight(xmlNodePtr Node)
	{
		std::string Type = GetProp(Node, "type");
		if (Type == "")
			throw XMLError("Light type not given");
		if (Type == "Ambient") {
			xmlNodePtr Cur = Node->xmlChildrenNode;
			OmitComments(Cur);
			EnsureColorTag(Cur, Node);
			Color C = ParseColor(Cur);
			Cur = Cur->next;
			OmitComments(Cur);
			if (Cur)
				throw XMLError(Node,
					"Garbage after color declaration");

			this->AddLight(new AmbientLight(C));
		} else if (Type == "Point") {
			xmlNodePtr Cur = Node->xmlChildrenNode;
			Bool GotColor = false;
			Bool GotPosition = false;
			Color C;
			Math::Vector Pos;
			OmitComments(Cur);
			for (; Cur != NULL;
			     Cur = Cur->next, OmitComments(Cur)) {
				if (!GotPosition && IsToken(Cur, "Position")) {
					Pos = ParseVector(Cur);
					GotPosition = true;
				}
				else if (!GotColor && IsToken(Cur, "Color")) {
					C = ParseColor(Cur);
					GotColor = true;
				} else {
					throw XMLError(Cur,
						"Garbage in pointlight"
						" declaration");
				}
			}
			if (!GotColor || !GotPosition)
				throw XMLError(Node,
					       "Point light requires color"
					       " and position data");
			this->AddLight(new PointLight(Pos, C));
		}
	}

	void Scene::ParseCamera(xmlNodePtr Node)
	{
		xmlNodePtr Cur = Node->xmlChildrenNode;
		Bool
			GotPos = false,
			GotDir = false,
			GotTop = false;

		Math::Vector
			Pos(0.0, 0.0, 0.0),
			Dir(0.0, 0.0, 1.0),
			Top(0.0, 1.0, 0.0);
		Double FOV = GetDoubleProp(Node, "FOV", 45.0);

		OmitComments(Cur);
		for (; Cur != NULL; Cur = Cur->next, OmitComments(Cur)) {
			if (!GotPos && IsToken(Cur, "Pos")) {
				Pos = ParseVector(Cur);
				GotPos = true;
			} else
			if (!GotDir && IsToken(Cur, "Dir")) {
				Dir = ParseVector(Cur);
				GotDir = true;
			} else
			if (!GotTop && IsToken(Cur, "Top")) {
				Top = ParseVector(Cur);
				GotTop = true;
			} else
				throw XMLError(
					"Garbage in camera"
					" declaration");
		}

		/* Create camera from read data */
		this->C = Camera(Pos,
				 Dir,
				 Camera::DegreeToFOV(FOV),
				 !GotTop,
				 Top);

	}

	void Scene::ParseSphere(xmlNodePtr Node)
	{
		xmlNodePtr Cur = Node->xmlChildrenNode;
		Bool	GotPosition = false,
			GotMaterial = false;
		const Material *Material = &MatLib::Gray();;
		Math::Vector Position(0.0, 0.0, 0.0);
		Double Radius = GetDoubleProp(Node, "radius");

		OmitComments(Cur);
		for (; Cur != NULL; Cur = Cur->next, OmitComments(Cur)) {
			if (!GotPosition && IsToken(Cur, "Position")) {
				Position = ParseVector(Cur);
				GotPosition = true;
			} else
			if (!GotMaterial && IsToken(Cur, "Material")) {
				std::string id = GetProp(Cur, "id");
				Material = GetMaterial(id);
				if (!Material)
					throw XMLError(Cur,
						"Material doesn't exist");
				GotMaterial = true;
			} else
				throw XMLError(
					"Garbage in Sphere"
					" declaration");
		}

		/* Create camera from read data */
		Sphere *S = new Sphere(Position, Radius, *Material);
		if (DEBUG)
			std::cout
				<< "Adding sphere " << *S << std::endl;
		this->AddObject(S);

	}

	void Scene::ParsePlane(xmlNodePtr Node)
	{
		xmlNodePtr Cur = Node->xmlChildrenNode;
		Bool	GotNormal = false,
			GotMaterial = false;
		const Material *Material = &MatLib::Gray();;
		Math::Vector Normal(0.0, 1.0, 0.0);
		Double Distance = GetDoubleProp(Node, "distance");

		OmitComments(Cur);
		for (; Cur != NULL; Cur = Cur->next, OmitComments(Cur)) {
			if (!GotNormal && IsToken(Cur, "Normal")) {
				Normal = ParseVector(Cur);
				GotNormal = true;
			} else
			if (!GotMaterial && IsToken(Cur, "Material")) {
				std::string id = GetProp(Cur, "id");
				Material = GetMaterial(id);
				if (!Material)
					throw XMLError(Cur,
						"Material doesn't exist");
				GotMaterial = true;
			} else
				throw XMLError(
					"Garbage in Plane"
					" declaration");
		}

		/* Create camera from read data */
		this->AddObject(
			new Plane(Normal, Distance, *Material));
	}

	Bool Scene::ParseFile(const std::string &File)
	{
		xmlDocPtr doc = NULL;
		xmlNodePtr cur;
		xmlLineNumbersDefault(1);
		xmlKeepBlanksDefault(0);
		try {
			doc = xmlParseFile(File.c_str());
			if (doc == NULL)
				throw XMLError("Unable to initialize parsing");

			cur = xmlDocGetRootElement(doc);
			if (cur == NULL)
				throw XMLError("Document is empty");

			if (xmlStrcmp(cur->name, (const xmlChar *) "Scene"))
				throw XMLError("Document has wrong type,"
					       " root node != Scene");

			/* Initialize scene object */
			ColMap.clear();
			MatMap.clear();
			TexMap.clear();
			CreateLibrary();

			cur = cur->xmlChildrenNode;
			OmitComments(cur);
			for (; cur != NULL;
			     cur = cur->next, OmitComments(cur)) {
				if (IsToken(cur, "Background")) {
					this->Background = ParseColor(cur);
					continue;
				}

				if (IsToken(cur, "Atmosphere")) {
					this->AtmosphereIdx = ParseIdx(cur);
					continue;
				}

				if (IsToken(cur, "Color")) {
					ParseColor(cur);
					continue;
				}

				if (IsToken(cur, "Texture")) {
					ParseTexture(cur);
					continue;
				}

				if (IsToken(cur, "Material")) {
					ParseMaterial(cur);
					continue;
				}

				if (IsToken(cur, "Light")) {
					ParseLight(cur);
					continue;
				}

				if (IsToken(cur, "Camera")) {
					ParseCamera(cur);
					continue;
				}

				if (IsToken(cur, "Sphere")) {
					ParseSphere(cur);
					continue;
				}

				if (IsToken(cur, "Plane")) {
					ParsePlane(cur);
					continue;
				}

				if (IsToken(cur, "Dump")) {
					std::cout << "*** Dump requested ***" << std::endl;
					DumpLibrary();
					continue;
				}


				throw XMLError("Invalid token in file \""
					       + ToStr(cur->name) + "\"");
			}
			xmlFreeDoc(doc);
			return true;
		} catch (std::exception &e) {
			xmlFreeDoc(doc);
			std::cout << "*** Error while parsing XML file:" << std::endl
				  << e.what() << std::endl;
			if (DEBUG) DumpLibrary();
			return false;
		}

	}
}
