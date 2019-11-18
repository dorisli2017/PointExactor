#ifndef POIREADER_H
#define POIREADER_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <set>

#include <osmium/osm/node.hpp>
#include <osmium/osm/types.hpp>

#include "projection.h"

class POI
{
public:
    /* Constructs the POI
     *
     * Arguments:
     *  label:      The string to label the POI with
     *  pos:        The position of the POI's anchor point
     *  classes:    A list of classes that this POI belongs to. Used for filtering.
     */
    POI(const std::string &label, Position pos, std::map<std::string, std::string> classes, osmium::object_id_type id);

    Position getPos();
    const std::string& getLabel();
    std::string getClass(std::string key) const;

    std::map<std::string, std::string> getClasses() const;
    bool matchFilter(std::map<std::string, std::set<std::string> > &filter);

    static const std::set<std::string> CLASS_KEYS;

    osmium::object_id_type getId() const;

    long getInternalId() const;
    static long getMaxIntenalId();
private:
    const std::string label;
    Position pos;
    osmium::object_id_type id;
    std::map<std::string, std::string> classes;

    long internal_id;
    static long MAX_ID;
};

class POIReader
{
public:
    POIReader(const std::string& osmFile);
    ~POIReader();

    void run() {readOSM();}

    int numPOIs() {return POIs.size();}
    POI& getPOI(const int& i) {return POIs[i];}
    std::vector<POI>& getPois() {return POIs;}

private:
    const std::string osmFile;
    std::vector<POI> POIs;
    std::set<std::string> foundNames;

    MercatorProjection proj;
    void readOSM();
    void parseOSMNode(osmium::Node &node);
};

#endif // POIREADER_H
