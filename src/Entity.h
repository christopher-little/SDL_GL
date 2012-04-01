/*
 * Entity.h
 *
 *  Created on: 2012-03-31
 *      Author: chris
 */

#ifndef ENTITY_H_
#define ENTITY_H_

namespace tank {

class Entity {
public:
	Entity();
	virtual ~Entity();


	int x, y; // Top,left position
	int w, h; // width,height of object in "world" space
	//***A graphic. Ref to GL texture or image data array?
};

} /* namespace tank */
#endif /* ENTITY_H_ */
