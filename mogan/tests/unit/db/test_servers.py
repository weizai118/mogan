# Copyright 2016 Intel
# All Rights Reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at
#
#         http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
#    License for the specific language governing permissions and limitations
#    under the License.

"""Tests for manipulating Servers via the DB API"""

from oslo_utils import uuidutils
import six

from mogan.common import exception
from mogan.tests.unit.db import base
from mogan.tests.unit.db import utils


class DbServerTestCase(base.DbTestCase):

    def test_server_create(self):
        utils.create_test_server()

    def test_server_create_with_same_uuid(self):
        utils.create_test_server(uuid='uuid', name='server1')
        self.assertRaises(exception.ServerAlreadyExists,
                          utils.create_test_server,
                          uuid='uuid',
                          name='server2')

    def test_server_get_by_uuid(self):
        server = utils.create_test_server()
        res = self.dbapi.server_get(self.context, server.uuid)
        self.assertEqual(server.uuid, res.uuid)

    def test_server_get_not_exist(self):
        self.assertRaises(exception.ServerNotFound,
                          self.dbapi.server_get,
                          self.context,
                          '12345678-9999-0000-aaaa-123456789012')

    def test_server_get_all(self):
        servers_project_1 = []
        servers_project_2 = []
        servers_project_all = []
        for i in range(0, 3):
            server = utils.create_test_server(
                uuid=uuidutils.generate_uuid(),
                project_id='project_1',
                name=str(i))
            servers_project_1.append(server)
        for i in range(3, 5):
            server = utils.create_test_server(
                uuid=uuidutils.generate_uuid(),
                project_id='project_2',
                name=str(i))
            servers_project_2.append(server)
        servers_project_all.extend(servers_project_1)
        servers_project_all.extend(servers_project_2)

        # Set project_only to False
        # get all servers from all projects
        res = self.dbapi.server_get_all(self.context, project_only=False)
        for i, item in enumerate(res):
            self.assertEqual(servers_project_all[i].uuid, item.uuid)

        # Filter by server name test
        res = self.dbapi.server_get_all(self.context, project_only=False,
                                        filters={"name": "1"})
        self.assertEqual(len(res), 1)
        self.assertEqual(res[0].name, "1")

        # Filter by server status test
        res = self.dbapi.server_get_all(self.context, project_only=False,
                                        filters={"status": "active"})
        for i, item in enumerate(res):
            self.assertEqual(item.status, "active")

        # Filter by flavor_uuid test
        res = self.dbapi.server_get_all(self.context, project_only=False,
                                        filters={"flavor_uuid":
                                                 servers_project_all[0].
                                                 flavor_uuid})
        for i, item in enumerate(res):
            self.assertEqual(item.flavor_uuid,
                             servers_project_all[0].flavor_uuid)

        # Filter by image_uuid test
        res = self.dbapi.server_get_all(self.context, project_only=False,
                                        filters={"image_uuid":
                                                 servers_project_all[0].
                                                 image_uuid})
        for i, item in enumerate(res):
            self.assertEqual(item.image_uuid,
                             servers_project_all[0].image_uuid)

        # Set project_only to True
        # get servers from current project (project_1)
        self.context.tenant = 'project_1'
        res = self.dbapi.server_get_all(self.context, project_only=True)
        res_uuids = [r.uuid for r in res]
        uuids_project_1 = [r.uuid for r in servers_project_1]
        six.assertCountEqual(self, uuids_project_1, res_uuids)

        # Set project_only to True
        # get servers from current project (project_2)
        self.context.tenant = 'project_2'
        res = self.dbapi.server_get_all(self.context, project_only=True)
        res_uuids = [r.uuid for r in res]
        uuids_project_2 = [r.uuid for r in servers_project_2]
        six.assertCountEqual(self, uuids_project_2, res_uuids)

    def test_server_destroy(self):
        server = utils.create_test_server()
        self.dbapi.server_destroy(self.context, server.uuid)
        self.assertRaises(exception.ServerNotFound,
                          self.dbapi.server_get,
                          self.context,
                          server.uuid)

    def test_server_destroy_not_exist(self):
        self.assertRaises(exception.ServerNotFound,
                          self.dbapi.server_destroy,
                          self.context,
                          '12345678-9999-0000-aaaa-123456789012')

    def test_server_update(self):
        server = utils.create_test_server()
        old_extra = server.extra
        new_extra = {'foo': 'bar'}
        self.assertNotEqual(old_extra, new_extra)

        res = self.dbapi.server_update(self.context,
                                       server.uuid,
                                       {'extra': new_extra})
        self.assertEqual(new_extra, res.extra)

    def test_server_update_with_invalid_parameter_value(self):
        server = utils.create_test_server()
        self.assertRaises(exception.InvalidParameterValue,
                          self.dbapi.server_update,
                          self.context,
                          server.uuid,
                          {'uuid': '12345678-9999-0000-aaaa-123456789012'})

    def test_tags_get_destroyed_after_destroying_a_server(self):
        server = utils.create_test_server(id=123)
        tag = utils.create_test_server_tag(self.context, server_id=server.id)

        self.assertTrue(self.dbapi.server_tag_exists(self.context,
                                                     server.id, tag.tag))
        self.dbapi.server_destroy(self.context, server.id)
        self.assertRaises(exception.ServerNotFound,
                          self.dbapi.server_tag_exists,
                          self.context, server.id, tag.tag)

    def test_tags_get_destroyed_after_destroying_a_server_by_uuid(self):
        server = utils.create_test_server(id=124)
        tag = utils.create_test_server_tag(self.context, server_id=server.id)

        self.assertTrue(self.dbapi.server_tag_exists(self.context,
                                                     server.id, tag.tag))
        self.dbapi.server_destroy(self.context, server.uuid)
        self.assertRaises(exception.ServerNotFound,
                          self.dbapi.server_tag_exists,
                          self.context, server.id, tag.tag)
