#include "ccGeoObject.h"

ccGeoObject::ccGeoObject(QString name, ccMainAppInterface* app)
	: ccHObject(name)
{
	//store reference to app so we can manipulate the database
	m_app = app;

	//add metadata tag defining the ccCompass class type
	QVariantMap* map = new QVariantMap();
	map->insert("ccCompassType", "GeoObject");
	setMetaData(*map, true);

	//add "interior", "upper" and "lower" HObjects
	generateInterior();
	generateUpper();
	generateLower();

}

void ccGeoObject::setType(QString type)
{
	//store attributes (centroid, strike, dip, RMS) on plane
	QVariantMap* map = new QVariantMap();
	map->insert("Type", type);
	setMetaData(*map, true);
}

QString ccGeoObject::getType()
{
	if (hasMetaData("Type"))
	{
		return getMetaData("Type").toString(); //return type
	}
	else
	{
		return "Unknown";
	}
}

ccPointCloud* ccGeoObject::getAssociatedCloud()
{
	return m_associatedCloud;
}

ccHObject* ccGeoObject::getRegion(int mappingRegion)
{
	switch (mappingRegion)
	{
	case ccGeoObject::INTERIOR:
		//check region hasn't been deleted...
		if (m_interior && !m_app->dbRootObject()->find(m_interior_id))
		{
			//item has been deleted... make a new one
			generateInterior();
		}
		return m_interior;
	case ccGeoObject::UPPER_BOUNDARY:
		//check region hasn't been deleted...
		if (m_upper && !m_app->dbRootObject()->find(m_upper_id))
		{
			//item has been deleted... make a new one
			generateUpper();
		}
		return m_upper;
	case ccGeoObject::LOWER_BOUNDARY:
		//check region hasn't been deleted...
		if (m_lower && !m_app->dbRootObject()->find(m_lower_id))
		{
			//item has been deleted... make a new one
			generateLower();
		}
		return  m_lower;
	default:
		return nullptr;
	}
}

void ccGeoObject::generateInterior()
{
	m_interior = new ccHObject("Interior");
	
	//give them associated property flags
	QVariantMap* map = new QVariantMap();
	map->insert("ccCompassType", "GeoInterior");
	m_interior->setMetaData(*map, true);

	//add these to the scene graph
	addChild(m_interior);
	m_interior_id = m_interior->getUniqueID();
}

void ccGeoObject::generateUpper()
{
	m_upper = new ccHObject("Upper Boundary");

	QVariantMap* map = new QVariantMap();
	map->insert("ccCompassType", "GeoUpperBoundary");
	m_interior->setMetaData(*map, true);

	addChild(m_upper);
	m_upper_id = m_upper->getUniqueID();
}

void ccGeoObject::generateLower()
{
	m_lower = new ccHObject("Lower Boundary");

	QVariantMap* map = new QVariantMap();
	map->insert("ccCompassType", "GeoLowerBoundary");
	m_interior->setMetaData(*map, true);

	addChild(m_lower);
	m_lower_id = m_lower->getUniqueID();
}


bool ccGeoObject::isGeoObject(ccHObject* object)
{
	if (object->hasMetaData("ccCompassType"))
	{
		return object->getMetaData("ccCompassType").toString().contains("GeoObject");
	}
	return false;
}
