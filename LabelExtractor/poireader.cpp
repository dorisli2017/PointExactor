#include "poireader.h"

#include <osmium/io/xml_input.hpp>

long POI::MAX_ID = 0;

POIReader::POIReader(const std::string& osmFile) :
    osmFile(osmFile)
{
}

POIReader::~POIReader()
{
}

void POIReader::readOSM()
{
    osmium::io::Reader reader(this->osmFile);
    osmium::io::Header header = reader.header();

    while (osmium::memory::Buffer buffer = reader.read()) {
        for (auto& entity : buffer) {
            switch (entity.type())
            {
            case osmium::item_type::node:
                this->parseOSMNode(static_cast<osmium::Node&>(entity));
                break;
            default:
                // We don't need anything else.
                break;
            }
        }
    }
}

void POIReader::parseOSMNode(osmium::Node &node)
{
    /* In the node_ref_list that will be available in the ways later, all locations are invalid.
     * Not sure if that is a bug or limitation, but that makes it necessary to store the locations
     * now. */

     if (node.tags().get_value_by_key("name") == nullptr) {
         // Things without names are no good
         return;
     }
    double x = (double)node.location().lon();
    double y = (double)node.location().lat();

    this->proj.project(&x, &y);

    std::map<std::string, std::string> classes;
    for (auto key : POI::CLASS_KEYS) {
        if (node.tags().get_value_by_key(key.c_str()) != nullptr) {
            classes.insert(std::make_pair(key, node.tags().get_value_by_key(key.c_str())));
        }
    }

    if (classes.size() == 0) {
        // No class key? Can't be selected!
        return;
    }

    bool validClass = false;
    for(const auto& c : POI::CLASS_KEYS)
        validClass |= (node.tags().get_value_by_key(c.c_str()) != nullptr);

    if (validClass) {
        auto ele = node.tags().get_value_by_key("ele");
//        if(!(classes.find("natural") != classes.end() && classes.at("natural") == "peak") || ele == nullptr || atoi(ele) < 2500)
//            return;

        std::string name = node.tags().get_value_by_key("name");
        if(this->foundNames.find(name) == this->foundNames.end()) {
            QPair<double, double> pos = qMakePair(x, y);
            POI poi(name, pos, classes, node.id());
            this->POIs.push_back(poi);
            this->foundNames.insert(name);
        }
    }
}

POI::POI(const std::string &label, Position pos, std::map<std::string, std::string> classes, osmium::object_id_type id):
    label(label), pos(pos), id(id), classes(classes)
{
  this->internal_id = POI::MAX_ID++;
}

long POI::getInternalId() const {
  return this->internal_id;
}

long POI::getMaxIntenalId() {
  return POI::MAX_ID;
}

Position POI::getPos()
{
    return this->pos;
}

const std::string &POI::getLabel()
{
    return this->label;
}

std::string POI::getClass(std::string key) const
{
    if (this->classes.find(key) != this->classes.end()) {
        return this->classes.at(key);
    } else {
        return "";
    }
}

std::map<std::string, std::string> POI::getClasses() const
{
    return classes;
}

bool POI::matchFilter(std::map<std::string, std::set<std::string> > &filter)
{
    if (filter.size() == 0) {
        return true;
    }

    for (auto key : this->classes) {
        if ((filter.find(key.first) != filter.end()) && (filter[key.first].find(key.second) != filter[key.first].end())) {
            return true;
        }
    }

    return false;
}

osmium::object_id_type POI::getId() const
{
    return id;
}

// TODO factor out
const std::set<std::string> POI::CLASS_KEYS = { "amenity", "building", "emergency", "historic", "leisure", "tourism", "place","natural"};
