.. -*- rst -*-

===========
 Instances
===========

Lists, creates, shows details for, updates, and deletes instances.

Create Instance
===============

.. rest_method:: POST /instances

Creates a instance.

The progress of this operation depends on the location of the
requested image, network I/O, selected type, and other factors.

The ``Location`` header returns the full URL to the newly created
instance and is available as a ``self`` and ``bookmark`` link in the
instance representation.

Normal response codes: 201

Error response codes: badRequest(400), unauthorized(401),
forbidden(403), conflict(409)

Request
-------

.. rest_parameters:: parameters.yaml

  - name: instance_name
  - description: instance_description
  - instance_type_uuid: typeRef
  - image_uuid: imageRef
  - availability_zone: availability_zone
  - networks: networks
  - networks.net_id: network_uuid
  - networks.port_type: network_port_type

**Example Create Instance: JSON request**

.. literalinclude:: samples/instances/instance-create-req.json
   :language: javascript

Response
--------

.. rest_parameters:: parameters.yaml

  - name: instance_name
  - description: instance_description
  - instance_type_uuid: typeRef
  - image_uuid: imageRef
  - availability_zone: availability_zone
  - network_info: network_info
  - links: links
  - uuid: instance_uuid
  - status: instance_status
  - power_state: instance_power_state
  - project_id: project_id_body
  - user_id: user_id_body
  - updated_at: updated_at
  - created_at: created_at
  - extra: extra

**Example Create Instance: JSON response**

.. literalinclude:: samples/instances/instance-create-resp.json
   :language: javascript


List Instances
===============

.. rest_method:: GET /instances

Return a list of bare metal Instances, with some useful information about each
Instance.

By default, this query will return the name, instance uuid, instance status
and description for each Instance.

Normal response codes: 200

Error response codes: badRequest(400), unauthorized(401),
forbidden(403)

Request
-------

.. rest_parameters:: parameters.yaml

  - all_tenants: all_tenants
  - fields: fields

Response
--------

.. rest_parameters:: parameters.yaml

  - name: instance_name
  - description: instance_description
  - uuid: instance_uuid
  - status: instance_status
  - links: links

**Example List of Instances: JSON response**

.. literalinclude:: samples/instances/instance-list-resp.json
   :language: javascript


List Instances Detailed
=======================

.. rest_method:: GET /instances/detail

Return a list of bare metal Instances with complete details.

Normal response codes: 200

Error response codes: badRequest(400), unauthorized(401),
forbidden(403)

Request
-------

  - all_tenants: all_tenants

Response
--------

.. rest_parameters:: parameters.yaml

  - name: instance_name
  - description: instance_description
  - instance_type_uuid: typeRef
  - image_uuid: imageRef
  - availability_zone: availability_zone
  - network_info: network_info
  - links: links
  - uuid: instance_uuid
  - status: instance_status
  - power_state: instance_power_state
  - project_id: project_id_body
  - user_id: user_id_body
  - updated_at: updated_at
  - created_at: created_at
  - launched_at: launched_at
  - extra: extra

**Example Detailed list of Instances: JSON response**

.. literalinclude:: samples/instances/instance-list-detail-resp.json
   :language: javascript


Show Instance Details
=====================

.. rest_method:: GET /instances/{instance_ident}

Shows details for a instance. By default, this will return the full
representation of the resource; an optional fields parameter can be supplied to
return only the specified set.

Normal response codes: 200

Error response codes: badRequest(400), unauthorized(401),
forbidden(403), itemNotFound(404)

Request
-------

.. rest_parameters:: parameters.yaml

  - instance_ident: instance_ident
  - fields: fields

Response
--------

.. rest_parameters:: parameters.yaml

  - name: instance_name
  - description: instance_description
  - instance_type_uuid: typeRef
  - image_uuid: imageRef
  - availability_zone: availability_zone
  - network_info: network_info
  - links: links
  - uuid: instance_uuid
  - status: instance_status
  - power_state: instance_power_state
  - project_id: project_id_body
  - user_id: user_id_body
  - updated_at: updated_at
  - created_at: created_at
  - launched_at: launched_at
  - extra: extra

**Example Instance Details: JSON response**

.. literalinclude:: samples/instances/instance-detail-resp.json
   :language: javascript


Update Instance
===============

.. rest_method:: PATCH /instances/{instances_uuid}

Updates the infromation stored about an instance.

Normal response codes: 200

Error response codes: badRequest(400), unauthorized(401),
forbidden(403), conflict(409)

Request
-------

The BODY of the PATCH request must be a JSON PATCH document, adhering to
`RFC 6902 <https://tools.ietf.org/html/rfc6902>`_.

.. rest_parameters:: parameters.yaml

  - instance_ident: instance_ident

**Example Update Instance: JSON request**

.. literalinclude:: samples/instances/instance-update-req.json
   :language: javascript

Response
--------

.. rest_parameters:: parameters.yaml

  - name: instance_name
  - description: instance_description
  - instance_type_uuid: typeRef
  - image_uuid: imageRef
  - availability_zone: availability_zone
  - network_info: network_info
  - links: links
  - uuid: instance_uuid
  - status: instance_status
  - power_state: instance_power_state
  - project_id: project_id_body
  - user_id: user_id_body
  - updated_at: updated_at
  - created_at: created_at
  - extra: extra

**Example Update Instance: JSON response**

.. literalinclude:: samples/instances/instance-update-resp.json
   :language: javascript


Delete Instance
===============

.. rest_method:: DELETE /instances/{instances_uuid}

Deletes an instance.

Preconditions

- The instance must exist.

Normal response codes: 204

Error response codes: unauthorized(401), forbidden(403), itemNotFound(404)

Request
-------

.. rest_parameters:: parameters.yaml

  - instance_ident: instance_ident

Response
--------

No body content is returned on a successful DELETE.


===============================
Instance Management (instances)
===============================

Instances can be managed through several sub-resources.

A Instance can be rebooted, turned on, or turned off by requesting a change to
its power state. This is handled asynchronously and tracked in the
``target_power_state`` field after the request is received.


Instance State Summary
======================

.. rest_method:: GET /v1/instances/{instance_ident}/states

Get a summary of the Instance's current power status.

Normal response code: 200

Request
-------

.. rest_parameters:: parameters.yaml

    - instance_ident: instance_ident

Response
--------

.. rest_parameters:: parameters.yaml

    - power_state: power_state
    - target_power_state: target_power_state

**Example instance state:**

.. literalinclude:: samples/instances/instance-get-state-response.json


Change Instance Power State
===========================

.. rest_method:: PUT /v1/instances/{instance_ident}/states/power

Request a change to the Instance's power state.

Normal response code: 202

Error codes:
    - 409 (ClientError)
    - 400 (InvalidState)
    - 406 (NotAcceptable)

Request
-------

.. rest_parameters:: parameters.yaml

    - instance_ident: instance_ident
    - target: power_state

**Example request to power off a Instance:**

.. literalinclude:: samples/instances/instance-set-power-off.json