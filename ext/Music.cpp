/*
 * Music.cpp
 *
 *  Created on: 28.10.2013
 *      Author: hanmac
 */

#include "Music.hpp"
#include "SoundStream.hpp"
#include "Time.hpp"

#define _self unwrap<sf::Music*>(self)

VALUE rb_cSFMLMusic;

template <>
VALUE wrap< sf::Music >(sf::Music *image )
{
	return Data_Wrap_Struct(rb_cSFMLMusic, NULL, free, image);
}

template <>
sf::Music* unwrap< sf::Music* >(const VALUE &vimage)
{
	return unwrapPtr<sf::Music>(vimage, rb_cSFMLMusic);
}

template <>
sf::Music& unwrap< sf::Music& >(const VALUE &vimage)
{
	return *unwrap<sf::Music*>(vimage);
}


namespace RubySFML {
namespace Music {

VALUE _alloc(VALUE self) {
	return wrap(new sf::Music);
}

macro_attr(Loop,bool)
macro_attr(PlayingOffset,sf::Time)

singlefunc(play)
singlefunc(pause)
singlefunc(stop)

singlereturn(getDuration)

VALUE _classloadFile(VALUE self,VALUE path)
{
	sf::Music *music = new sf::Music;

	if(music->openFromFile(unwrap<std::string>(path)))
		return wrap(music);
	return Qnil;
}


VALUE _loadFile(VALUE self,VALUE path)
{
	return wrap(_self->openFromFile(unwrap<std::string>(path)));
}

/*
 *
 */
VALUE _getStatus(VALUE self)
{
	return wrapenum(_self->getStatus());
}

}
}

void Init_SFMLMusic(VALUE rb_mSFML)
{
	using namespace RubySFML::Music;

#if 0
	rb_mSFML = rb_define_module("SFML");

#endif

	rb_cSFMLMusic = rb_define_class_under(rb_mSFML,"Music",rb_cSFMLSoundStream);

	rb_define_alloc_func(rb_cSFMLMusic,_alloc);

	rb_define_attr_method(rb_cSFMLMusic,"loop",_getLoop,_setLoop);
	rb_define_attr_method(rb_cSFMLMusic,"playing_offset",_getPlayingOffset,_setPlayingOffset);

	rb_undef_method(rb_cSFMLMusic,"_dump");
	rb_undef_method(rb_cSFMLMusic,"_load");

	rb_define_method(rb_cSFMLMusic,"play",RUBY_METHOD_FUNC(_play),0);
	rb_define_method(rb_cSFMLMusic,"pause",RUBY_METHOD_FUNC(_pause),0);
	rb_define_method(rb_cSFMLMusic,"stop",RUBY_METHOD_FUNC(_stop),0);

	rb_define_singleton_method(rb_cSFMLMusic,"load_file",RUBY_METHOD_FUNC(_classloadFile),1);
	rb_define_method(rb_cSFMLMusic,"load_file",RUBY_METHOD_FUNC(_loadFile),1);

	rb_define_method(rb_cSFMLMusic,"status",RUBY_METHOD_FUNC(_getStatus),0);

	rb_define_method(rb_cSFMLMusic,"duration",RUBY_METHOD_FUNC(_getDuration),0);

}

