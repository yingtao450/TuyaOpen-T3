.. _bk_config_ota:

OTA 配置
========================

:link_to_translation:`en:[English]`

OTA 用于配置 OTA 策略与防版本降级攻击的安全计数器。

OTA 配置表
----------------------------

OTA 配置表为 CSV 文件，下面为一个配置示例：

+-------------------------+--------------------+
| Field                   | Value              |
+=========================+====================+
| strategy                | SWAP               |
+-------------------------+--------------------+
| app_security_counter    | 3                  |
+-------------------------+--------------------+

分区表固定为两列：

 - ``strategy`` - 可选项，表示 MCUBOOT OTA 时策略，可取 ``SWAP``, ``XIP``或者 ``OVERWRITE`` ，分别对应 MCUBOOT 中的 MCUBOOT_SWAP_USING_MOVE, MCUBOOT_DIRECT_XIP
   与 MCUBOOT_OVERWRITE_ONLY 三种策略。
 - ``app_security_counter`` - 可选项，表示版本安全计数器。安全启动使能时必须配置，用于防止版本降级攻击。安全启动验证版本合法之后
   会读取该值，与 OTP/NV 中的安全计数器作比较，仅当升级版本的安全计数器不小于 OTP/NV 中的值时才允许启动。


.. _bk_config_ota_strategy:

OTA 升级策略与其对应的分区要求
-------------------------------------------

下面将为您说明基于 MCUBOOT 的三种升级策略,请参考分区配置文档 :ref:`分区表 <bk_config_partitions_table>` 一起理解。

SWAP
++++++++++++++++++++++++++++++
SWAP 中程序总是运行在主分区即 primary 分区,而待升级的固件会在 OTA 下载时放置在次分区即 secondary 分区,两者大小必须相同。升级时, MCUBOOT 会将次分区的内容与主分区进行交换,并且尝试运行
此时主分区中的固件,若可以成功运行,则会将此时主分区的内容保留,此后从主分区启动。若启动失败,则会将次分区的内容重新交换回主分区,确保您始终拥有一个可以运行的版本。

XIP
+++++++++++++++++++++++++++++
XIP 同样需要两个相同的分区,但两者不再具有主次之分,而是在升级时,会直接尝试从另一分区启动。例如当程序运行在 A 分区时,通过 OTA 下载了待升级的固件置入 B 分区,则 MCUBOOT 会尝试
运行 B 分区中的固件,若可以成功运行,则之后将保持在 B 分区运行。若失败,则 MCUBOOT 会重新从A分区运行之前的固件。同样可以确保您始终拥有要给可以运行的版本。为了统一,我们仍然保留了
`primary_` 和 `secondary_` 前缀,且将 primary 分区作为A分区,secondary 分区作为B分区。

OVERWRITE
++++++++++++++++++++++++++++
OVERWRITE 中只有一个 primary 分区用来运行固件,且没有 secondary 分区。我们使用了 ota 分区来存放待升级的固件,且该分区大小可以显著小于主分区。在 OVERWRITE 策略中,我们将待升级的固件压缩并
放入 ota 分区中,该分区并不作为可执行分区,而是在升级时解压并且覆写 primary 分区,程序仍然在 primary 分区运行。
使用该策略有一个缺点和一个优点,缺点是如果待升级的固件无法运行,则板子将变砖。在该策略下只能保证待升级固件的合法性,即该固件是完整的、没有被篡改的、安全的,但是无法保证固件本身是
没有故障可以运行的。而优点是可以节省下许多空间, ota 分区的大小可以根据待升级的固件压缩后的大小来决定。我们推荐您将 ota 分区的大小设置为所有 primary 分区大小总和的50%。

.. warning::
  使用 OVERWRITE 策略时请务必确保升级后的固件是可以运行的,否则将无法挽回!