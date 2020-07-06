<template>
  <v-card class="card" elevation="0">
    <v-list-item @click="$emit('view')">
      <v-list-item-content>
        <v-list-item-title>
          {{ file.name }} [{{ file.size }}]
        </v-list-item-title>

        <v-list-item-subtitle>
          {{ file.path }}
        </v-list-item-subtitle>
      </v-list-item-content>

      <v-list-item-action>
        <v-icon color="red" @click="$emit('remove')">
          mdi-delete
        </v-icon>
      </v-list-item-action>
    </v-list-item>
  </v-card>
</template>

<script>
export default {
  name: "InputFile",

  props: {
    file: {
      type: Object,
      required: true
    }
  }
}
</script>

<style lang="sass" scoped>
.card
  margin: 0 !important
  padding: 0 !important
  border: 1px solid rgba(black, .14) !important
</style>
