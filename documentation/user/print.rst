Displaying the content of a DICOM file
======================================

The command ``odil print`` displays the content of a `DICOM file`_. For each element, it will display its `name`_ and its `tag`_, its `VR`_ (value representation, i.e. type), and its value. 

.. code-block:: console

  $ odil print file.dcm
  Specific Character Set    0008,0005 CS ['ISO_IR 100']
  ...
  SOP Class UID             0008,0016 UI ['1.2.840.10008.5.1.4.1.1.4']
  ...
  0019,0010                 0019,0010 LO ['SIEMENS MR HEADER']
  0019,1008                 0019,1008 CS ['IMAGE NUM 4']
  ...
  Image Position (Patient)  0020,0032 DS [-32.0, -120.3125, 175.0]
  ...
  Pixel Data                7fe0,0010 OW (binary, 1 item, 90112 bytes)

Text and numeric elements, even when nested in other elements, will display their full value. Binary elements (for example the pixel data) cannot be easily displayed, and are only shown as a summary containing the number of items and the size in bytes.

Note that for unknown elements (like the private element *0019,1008* in the above example), no name can be displayed, and the tag is used instead. The value is however displayed as for known elements.

Displaying the name of UIDs
---------------------------

By default, the `UIDs`_ (like the value of *SOP Class UID*) are displayed as their value (*1.2.840.10008.5.1.4.1.1.4*) and not as their name (*MR Image Storage*). This behavior can be changed with the ``-u`` option:

.. code-block:: console

  $ odil print -u file.dcm
  Specific Character Set    0008,0005 CS ['ISO_IR 100']
  ...
  SOP Class UID             0008,0016 UI ['MR Image Storage']
  ...

The name will only be displayed for `registered UIDs`_.

Displaying the meta information
-------------------------------

Only elements in the data set (i.e. where the group is higher than *0008*) are displayed by default. To display the `meta information`_ or *header* of the file, use the `-H` flag:

.. code-block:: console

  $ odil print -H file.dcm
  File Meta Information Version  0002,0001 OB (binary, 1 item, 2 bytes)
  Media Storage SOP Class UID    0002,0002 UI ['1.2.840.10008.5.1.4.1.1.4']
  ...
  Transfer Syntax UID            0002,0010 UI ['1.2.840.10008.1.2.1']
  ...

  Specific Character Set         0008,0005 CS ['ISO_IR 100']
  ...

Displaying multiple files
-------------------------

Multiple files may be specified, in which case their respective contents will be displayed in order:

.. code-block:: console

  $ odil print file1.dcm file2.dcm ... fileN.dcm
  ...
  SOP Instance UID  0008,0018 UI ['1.3.12.2.1107.5.2.30.25842.35105']
  ...
  SOP Instance UID  0008,0018 UI ['1.3.12.2.1107.5.2.30.25842.35125']
  ...
  SOP Instance UID  0008,0018 UI ['1.3.12.2.1107.5.2.30.25842.35109']
  ...

.. _DICOM file: http://dicom.nema.org/medical/dicom/current/output/chtml/part10/chapter_7.html
.. _meta information: http://dicom.nema.org/medical/dicom/current/output/chtml/part10/chapter_7.html#sect_7.1
.. _name: http://dicom.nema.org/medical/dicom/current/output/chtml/part06/chapter_6.html
.. _registered UIDs: http://dicom.nema.org/medical/dicom/current/output/chtml/part06/chapter_A.html
.. _tag: http://dicom.nema.org/medical/dicom/current/output/chtml/part06/chapter_5.html
.. _UIDs: http://dicom.nema.org/medical/dicom/current/output/chtml/part05/chapter_9.html
.. _VR: http://dicom.nema.org/medical/dicom/current/output/chtml/part05/sect_6.2.html
