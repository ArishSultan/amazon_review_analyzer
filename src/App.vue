<template>
  <v-app>
    <v-app-bar
      app
      color="primary"
      dark
    >
      [This is Preview Build]
    </v-app-bar>

    <v-main>
      <v-container style="display: flex; justify-content: center">
        <files-input />
      </v-container>
    </v-main>
  </v-app>
</template>

<script>
import FilesInput from "./components/FilesInput";

export default {
  name: 'App',

  components: {
    FilesInput
  },

  data: () => ({
    //
  }),
};
</script>
