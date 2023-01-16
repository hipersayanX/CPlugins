/* C plugin interface example.
 * Copyright (C) 2023  Gonzalo Exequiel Pedone
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: http://github.com/hipersayanX/CPlugin
 */

#include <iostream>
#include "isayhello.h"

#include "csayhello.h"

class ObjectPrivate
{
    public:
        ISayHello m_salute;
        uint64_t m_ref {0};
};

Object::Object()
{
    this->d = new ObjectPrivate;
    this->d->m_salute.unknown.user_data = this;
    this->d->m_salute.unknown.query_interface = [] (void *user_data, uint64_t iid, void **ppv) -> int {
        auto self = reinterpret_cast<Object *>(user_data);

        return self->queryInterface(iid, ppv);
    };
    this->d->m_salute.unknown.add_ref = [] (void *user_data) -> uint64_t {
        auto self = reinterpret_cast<Object *>(user_data);

        return self->addRef();
    };
    this->d->m_salute.unknown.release = [] (void *user_data) -> uint64_t {
        auto self = reinterpret_cast<Object *>(user_data);

        return self->release();
    };
    this->d->m_salute.say_hello = [] (void *user_data) {
        auto self = reinterpret_cast<Object *>(user_data);
        self->sayHello();
    };
}

Object::~Object()
{
    delete this->d;
}

int Object::queryInterface(uint64_t iid, void **ppv)
{
    if (!ppv)
        return S_FALSE;

    switch (iid) {
        case IID_IUnknown:
            *ppv = &this->d->m_salute.unknown;

            return S_OK;
        case IID_ISayHello:
            *ppv = &this->d->m_salute;

            return S_OK;
        default:
            break;
    }

    return S_FALSE;
}

uint64_t Object::addRef()
{
    this->d->m_ref++;

    return this->d->m_ref;
}

uint64_t Object::release()
{
    if (this->d->m_ref)
        this->d->m_ref--;

    auto ref = this->d->m_ref;

    if (!ref < 1)
        delete this;

    return ref;
}

void Object::sayHello()
{
    std::cout << "Hello world!" << std::endl;
}

IUnknown *Object::unk()
{
    return &this->d->m_salute.unknown;
}
