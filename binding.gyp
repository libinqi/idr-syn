{
  "targets": [
    {
      "target_name": "SynIDCard",
      "sources": [ "src/SynIDCard.cc"],
      "include_dirs": [
      'tmp/include',
	  'src'
      ],
      'link_settings': {
        'libraries': [
          '-lSynIDCardAPI'
        ],
        'library_dirs': [
          'tmp/lib'
        ]
      },
	  "msbuild_settings": {
			"Link": {
				"ImageHasSafeExceptionHandlers": "false"
			}
		}
    }
  ]
}
